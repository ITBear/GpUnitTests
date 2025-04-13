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

void    GpUnitTestManager::SetHandlerFactory (GpUnitTestHandlerFactory::SP aFactory)
{
    GpUniqueLock<GpMutex> uniqueLock{iMutex};

    VERIFY
    (
        iIsRun == false,
        "Failed to set handler factory. GpUnitTestManager RUN in progress..."_sv
    );

    iHandlerFactory = std::move(aFactory);
}

void    GpUnitTestManager::RunAndWaitForDone (void)
{
    GpUnitTestHandler::SP managerHandlerSP;

    {
        GpUniqueLock<GpMutex> uniqueLock{iMutex};

        if (iIsRun) [[unlikely]]
        {
            return;
        }

        iIsRun = true;

        if (iHandlerFactory.IsNULL())
        {
            iHandlerFactory = MakeSP<GpUnitTestLogOutHandlerFactory>();
        }

        managerHandlerSP = iHandlerFactory.V().NewInstance();
    }   

    GpUnitTestHandler& managerHandler = managerHandlerSP.V();

    managerHandler.OnManagerStart();

    std::vector<GpUnitTestHandlerStatistics> statistics;
    std::atomic_bool isProduceDone = false;

    SharedQueueT::SP        sharedQueue             = MakeSP<SharedQueueT>(size_t{300});
    DoneFutureT::C::Vec::SP testRunnerDoneFutures   = StartRunners(sharedQueue, statistics, isProduceDone);

    ProduceTests(sharedQueue.V(), isProduceDone);
    WaitForRunners(testRunnerDoneFutures);

    const bool isNoFailedTests = OnDone(statistics, managerHandler);

    {
        GpUniqueLock<GpMutex> uniqueLock{iMutex};
        iIsRun = false;
    }

    if (!isNoFailedTests)
    {
        GpService::SSetResultCode(EXIT_FAILURE);
    }
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
    GpUniqueLock<GpMutex> uniqueLock{iMutex};

    VERIFY
    (
        iIsRun == false,
        [&]()
        {
            return fmt::format
            (
                "Failed to add unit test group '{}'. GpUnitTestManager RUN in progress...",
                aUnitTestSuiteGroupTypeDemangleName
            );
        }
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

GpUnitTestManager::DoneFutureT::C::Vec::SP  GpUnitTestManager::StartRunners
(
    SharedQueueT::SP&                           aSharedQueue,
    std::vector<GpUnitTestHandlerStatistics>&   aStatistics,
    std::atomic_bool&                           aIsProduceDoneRef
) NO_THREAD_SAFETY_ANALYSIS
{
    DoneFutureT::C::Vec::SP testRunnerDoneFutures;
    const size_t            executorsCount = GpTaskScheduler::S().ExecutorsCount();

    testRunnerDoneFutures.reserve(executorsCount);  
    aStatistics.resize(executorsCount);

    for (size_t id = 0; id < executorsCount; id++)
    {
        GpUnitTestRunner::SP testRunner;

        testRunner = MakeSP<GpUnitTestRunner>
        (
            id,
            aIsProduceDoneRef,
            aSharedQueue,
            iHandlerFactory,
            aStatistics[id]
        );

        auto donePromiseOptSP = GpTaskScheduler::S().NewToReadyDepend(std::move(testRunner));

        VERIFY
        (
            donePromiseOptSP.has_value(),
            "Failed to start TEST task"
        );

        testRunnerDoneFutures.emplace_back
        (
            std::move(donePromiseOptSP.value())
        );
    }

    return testRunnerDoneFutures;
}

void    GpUnitTestManager::ProduceTests
(
    SharedQueueT&       aSharedQueue,
    std::atomic_bool&   aIsProduceDoneRef
) NO_THREAD_SAFETY_ANALYSIS
{
    GpDoOnceInPeriod                onceInPeriod(253.0_si_ms, GpDoOnceInPeriod::Mode::AT_TIMEOUT);
    std::atomic_size_t              runingGroupsCount   = 0;
    const GpUnitTestAppCmdArgsDesc& cmdArgsDesc         = GpUnitTestAppCmdArgsDesc::SGet();
    std::string_view                unitTestFilter      = cmdArgsDesc.unit_test_filter;

    for (auto&[_, testGroupSP]: iTestGroups)
    {
        GpUnitTestGroup& testGroup = testGroupSP.Vn();

        if (testGroup.RunMode() == GpUnitTestGroupRunMode::RUN_EXCLUSIVE)
        {
            // Wait for other groups done
            while (   (!aSharedQueue.Empty())
                   || (runingGroupsCount > 0))
            {
                YIELD_READY_TO_RUN();
            }
        }

        //  Filter out tests
        bool isNeedToRunTestsGroup = true;
        if (!unitTestFilter.empty())
        {
            std::regex  filterByNameRgex = StrOps::SPrepareRegexFilter(unitTestFilter);
            std::smatch filterByNameRgexMatch;

            isNeedToRunTestsGroup = false;

            //  Filter out tests group
            const std::string testGroupName{testGroup.Name()};
            if (std::regex_match(testGroupName, filterByNameRgexMatch, filterByNameRgex))
            {
                isNeedToRunTestsGroup = true;
            } else
            {
                // Filter out individual tests
                const size_t testsCountAfterFilter = testGroup.FilterTests(unitTestFilter);
                isNeedToRunTestsGroup = testsCountAfterFilter > 0;
            }
        }

        if (isNeedToRunTestsGroup)
        {
            testGroup.SetRunCounterAndInc(runingGroupsCount);

            while (!aSharedQueue.PushAndNotifyAll(testGroupSP))
            {
                LOG_INFO("UNIT TEST manager Produce/Consume queue is full, waiting..."_sv);
                YIELD_READY_TO_RUN();
            }
        }

        onceInPeriod.Do
        (
            []()
            {
                YIELD_READY_TO_RUN();
            }
        );
    }

    aIsProduceDoneRef.store(true, std::memory_order_release);
}

void    GpUnitTestManager::WaitForRunners (DoneFutureT::C::Vec::SP& aTestRunnerDoneFutures)
{
    while (!aTestRunnerDoneFutures.empty())
    {
        for (auto iter = std::begin(aTestRunnerDoneFutures); iter != std::end(aTestRunnerDoneFutures); )
        {
            const bool isReady = GpItcFutureUtils::STryCheck
            (
                iter->V(),
                [](typename GpTaskFiber::DoneFutureT::value_type& /*aRes*/)
                {
                    LOG_INFO("[GpUnitTestManager::WaitForRunners]: done"_sv);
                },
                [](const GpException& aException)
                {
                    const std::string msg = fmt::format
                    (
                        "[GpUnitTestManager::WaitForRunners]: Unit Test Runner finished with error: {}",
                        aException.what()
                    );

                    LOG_ERROR(msg);
                    THROW(msg, aException.SourceLocation());
                }
            );

            if (isReady)
            {
                iter = aTestRunnerDoneFutures.erase(iter);
            } else
            {
                iter++;
            }
        }

        YIELD_READY_TO_RUN();
    }
}

bool    GpUnitTestManager::OnDone
(
    const std::vector<GpUnitTestHandlerStatistics>& aStatistics,
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
