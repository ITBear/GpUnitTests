#include <GpCore2/Config/IncludeExt/fmt.hpp>
#include <GpLog/GpLogCore/GpLog.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpCore2/GpUtils/Exceptions/GpExceptionUtils.hpp>
#include <GpCore2/GpTasks/ITC/GpItcFutureUtils.hpp>
#include <GpUnitTests/GpUnitTestManager.hpp>
#include <GpUnitTests/GpUnitTestRunner.hpp>
#include <GpUnitTests/Handlers/GpUnitTestLogOutHandlerFactory.hpp>
#include <GpUnitTests/AppService/GpUnitTestAppCmdArgsDesc.hpp>
#include <GpService/GpService.hpp>

#include <numeric>

namespace GPlatform::UnitTest {

GpUnitTestManager::GpUnitTestManager (void) noexcept
{
}

GpUnitTestManager::~GpUnitTestManager (void) noexcept
{
}

GpUnitTestManager&  GpUnitTestManager::S (void)
{
    static GpUnitTestManager sInstance;
    return sInstance;
}

void    GpUnitTestManager::RunAndWaitForDone (void) noexcept
{
    GpDefer defer = [&]()
    {
        GpUniqueLock uniqueLock{iMutex};
        iIsEnableToAdd = true;
    };

    {
        GpUniqueLock uniqueLock{iMutex};
        iIsEnableToAdd = false;
    }

    GpException exception;

    try
    {
        GpUnitTestHandler::UP   managerHandlerUP    = GpUnitTestLogOutHandlerFactory{}.NewInstance();
        GpUnitTestHandler&      managerHandler      = *managerHandlerUP;

        managerHandler.OnManagerStart();

        const size_t executorsCount = GpTaskScheduler::S().ExecutorsCount();
        VERIFY
        (
            executorsCount >= 2,
            "Executors count must be >= 2"
        );

        ExecQueueT::SP          execQueueSP             = MakeSP<ExecQueueT>(executorsCount - 1);
        DoneFutureT::C::Vec::SP testRunnerDoneFutures   = StartRunners(execQueueSP);

        VERIFY
        (
            !testRunnerDoneFutures.empty(),
            "An error occurred while creating the test runners"
        );

        // Run tests
        RunTests(execQueueSP);

        execQueueSP->StopProduce();

        // Wait for done
        const GpUnitTestHandlerStatistics::C::Vec::Val statistics = WaitForRunners(testRunnerDoneFutures);

        // Process results
        const bool isNoFailedTests = OnDone(statistics, managerHandler);

        if (!isNoFailedTests)
        {
            GpService::SSetResultCode(EXIT_FAILURE);
        }

        return;
    } catch (const GpException& ex)
    {
        exception = ex;
    } catch (const std::exception& ex)
    {
        exception = GpException{ex.what()};
    } catch (...)
    {
        exception = GpException{"[GpUnitTestManager::OnStop]: unknown exception"_sv};
    }

    LOG_EXCEPTION
    (
        "[GpUnitTestManager::RunAndWaitForDone]",
        exception
    );

    GpService::SSetResultCode(EXIT_FAILURE);
}

void    GpUnitTestManager::AddGroupTest
(
    UnitTestSuiteGroupFactoryFnT    aUnitTestSuiteGroupFactoryFn,
    std::string_view                aUnitTestSuiteGroupTypeDemangleName,
    GpUnitTest::FnT                 aTestFn,
    std::string                     aGroupName,
    std::string                     aTestName,
    std::string                     aTestCommment
)
{
    GpUniqueLock uniqueLock{iMutex};

    VERIFY
    (
        iIsEnableToAdd == true,
        "iIsEnableToAdd == false"
    );

    auto iter = iTestGroups.find(aUnitTestSuiteGroupTypeDemangleName);

    if (iter == std::end(iTestGroups))
    {
        iter = iTestGroups.emplace
        (
            std::string(aUnitTestSuiteGroupTypeDemangleName),
            MakeSP<GpUnitTestGroup>
            (
                std::move(aGroupName),
                aUnitTestSuiteGroupFactoryFn()
            )
        ).first;
    }

    GpUnitTestGroup::SP unitTestGroup = iter->second;
    unitTestGroup->AddTest
    (
        MakeSP<GpUnitTest>
        (
            std::move(aTestFn),
            std::move(aTestName),
            std::move(aTestCommment)
        )
    );
}

GpUnitTestManager::DoneFutureT::C::Vec::SP  GpUnitTestManager::StartRunners (ExecQueueT::SP aExecQueueSP) noexcept
{
    GpException exception;

    try
    {
        DoneFutureT::C::Vec::SP testRunnerDoneFutures;
        const size_t            executorsCount = GpTaskScheduler::S().ExecutorsCount();

        testRunnerDoneFutures.reserve(executorsCount);

        for (size_t id = 0; id < executorsCount; id++)
        {
            GpUnitTestRunner::SP testRunnerSP = MakeSP<GpUnitTestRunner>
            (
                id,
                aExecQueueSP
            );

            testRunnerDoneFutures.emplace_back
            (
                testRunnerSP.Vn().DoneFuture()
            );

            SPAWN_READY_TASK(testRunnerSP);
        }

        return testRunnerDoneFutures;
    } catch (const GpException& ex)
    {
        exception = ex;
    } catch (const std::exception& ex)
    {
        exception = GpException{ex.what()};
    } catch (...)
    {
        exception = GpException{"[GpUnitTestManager::OnStop]: unknown exception"_sv};
    }

    LOG_EXCEPTION
    (
        "[GpUnitTestManager::StartRunners]",
        exception
    );

    return {};
}

void    GpUnitTestManager::RunTests (ExecQueueT::SP aExecQueueSP) noexcept
{
    GpException exception;

    try
    {
        GpUnitTestGroup::C::MapStr::SP  testGroups;
        {
            GpUniqueLock uniqueLock{iMutex};
            testGroups = std::move(iTestGroups);
        }

        ExecQueueT& execQueue = aExecQueueSP.V();

        //std::atomic_size_t            runingGroupsCount   = 0;
        const GpUnitTestAppCmdArgsDesc& cmdArgsDesc         = GpService::SArgs().CastTo<GpUnitTestAppCmdArgsDesc::CSP>().V();
        std::string_view                unitTestFilter      = cmdArgsDesc.unit_test_filter;

        for (auto&[_, testGroupSP]: testGroups)
        {
            GpUnitTestGroup& testGroup = testGroupSP.Vn();

            // Filter out tests
            bool skipTest = false;
            if (!unitTestFilter.empty())
            {
                std::regex  filterByNameRgex = StrOps::SPrepareRegexFilter(unitTestFilter);
                std::smatch filterByNameRgexMatch;

                //  Filter out tests group
                const std::string testGroupName{testGroup.Name()};
                if (std::regex_match(testGroupName, filterByNameRgexMatch, filterByNameRgex))
                {
                    skipTest = false;
                } else
                {
                    // Filter out individual tests
                    const size_t testsCountAfterFilter = testGroup.FilterTests(unitTestFilter);
                    skipTest = testsCountAfterFilter == 0;
                }
            }

            if (skipTest) [[unlikely]]
            {
                continue;
            }

            while (true)
            {
                const auto flags = execQueue.PushWaitFor(testGroupSP, 250.0_si_ms);

                if (flags == 0) [[likely]]
                {
                    break;
                } else if (flags & (ExecQueueT::FlagsT(ExecQueueT::FlagE::INTERRUPT) | ExecQueueT::FlagsT(ExecQueueT::FlagE::STOP_PRODUCE)))
                {
                    return;
                }
            }
        }
    } catch (const GpException& ex)
    {
        exception = ex;
    } catch (const std::exception& ex)
    {
        exception = GpException{ex.what()};
    } catch (...)
    {
        exception = GpException{"[GpUnitTestManager::RunTests]: unknown exception"_sv};
    }

    LOG_EXCEPTION
    (
        "[GpUnitTestManager::StartRunners]",
        exception
    );
}

GpUnitTestHandlerStatistics::C::Vec::Val    GpUnitTestManager::WaitForRunners (DoneFutureT::C::Vec::SP& aTestRunnerDoneFutures)
{
    static const auto onExceptionFn = [](const GpException& aException)
    {
        const std::string msg = fmt::format
        (
            "[GpUnitTestManager::WaitForRunners]: Unit Test Runner finished with error: {}",
            aException.what()
        );

        LOG_ERROR(msg);
    };

    GpUnitTestHandlerStatistics::C::Vec::Val statistics;

    while (!aTestRunnerDoneFutures.empty())
    {
        for (auto iter = std::begin(aTestRunnerDoneFutures); iter != std::end(aTestRunnerDoneFutures); /*iter++*/)
        {
            auto& doneFuture = iter->V();

            const bool isReady = GpItcFutureUtils::SWaitFor
            (
                doneFuture,
                [&](typename GpTaskFiber::DoneFutureT::value_type&& aResult)
                {
                    auto stat = aResult.ValueMove().Value<GpUnitTestHandlerStatistics>();
                    statistics.emplace_back(stat);
                },
                onExceptionFn,
                250.0_si_ms,
                nullptr
            );

            if (isReady)
            {
                iter = aTestRunnerDoneFutures.erase(iter);
            } else
            {
                iter++;
            }
        }
    }

    LOG_INFO("[GpUnitTestManager::WaitForRunners]: All tests have finished"_sv);

    return statistics;
}

bool    GpUnitTestManager::OnDone
(
    const GpUnitTestHandlerStatistics::C::Vec::Val& aStatistics,
    GpUnitTestHandler&                              aManagerHandler
)
{
    GpUnitTestHandlerStatistics initStat;
    initStat.startTs = GpDateTimeOps::SUnixTS_ms();

    const GpUnitTestHandlerStatistics resultStat = std::accumulate
    (
        std::begin(aStatistics),
        std::end(aStatistics),
        initStat,
        [](const GpUnitTestHandlerStatistics& a, const GpUnitTestHandlerStatistics& b)
        {
            return GpUnitTestHandlerStatistics::SSumm(a, b);
        }
    );

    aManagerHandler.OnManagerDone(resultStat);

    return resultStat.IsNoFailed();
}

}// namespace GPlatform::UnitTest
