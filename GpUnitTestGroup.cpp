#include <GpUnitTests/GpUnitTestGroup.hpp>
#include <GpUnitTests/GpUnitTestAssert.hpp>
#include <GpUnitTests/Handlers/GpUnitTestHandler.hpp>

#include <GpCore2/GpUtils/DateTime/GpDateTimeOps.hpp>
#include <GpCore2/GpUtils/Other/GpRAIIonDestruct.hpp>
#include <regex>

namespace GPlatform::UnitTest {

GP_ENUM_IMPL(GpUnitTestGroupRunMode)

GpUnitTestGroup::GpUnitTestGroup (GpUnitTestSuiteGroup::SP aSuite) noexcept:
iSuite{std::move(aSuite)}
{
}

void    GpUnitTestGroup::OnTestFailedExpect
(
    std::string_view        aMsg,
    const SourceLocationT&  aLocation
)
{
    THROW_COND_GP
    (
        iCurrentHandler != nullptr,
        "iCurrentHandler is null"_sv
    );

    iCurrentHandler->OnTestFailedExpect
    (
        iSuite->Name(),
        iCurrentTestName,
        aMsg,
        aLocation,
        GpDateTimeOps::SSteadyTS_us() - iRunStartSTS
    );

    iTestExpectFailsCnt++;
}

void    GpUnitTestGroup::SetRunCounterAndInc (std::atomic_size_t& aRuningGroupsCountRef) noexcept
{
    iRuningGroupsCountOptRef = aRuningGroupsCountRef;
    iRuningGroupsCountOptRef.value().get()++;
}

size_t  GpUnitTestGroup::FilterTests (std::string_view aFilter)
{
    if (aFilter.empty())
    {
        return std::size(iTests);
    }

    std::regex  rgex = StrOps::SPrepareRegexFilter(aFilter);
    std::smatch match;

    for (auto iter = std::begin(iTests); iter != std::end(iTests); /**/)
    {
        const std::string testName{iter->V().Name()};

        if (std::regex_match(testName, match, rgex))
        {
            iter++;
        } else
        {
            iter = iTests.erase(iter);
        }
    }

    return std::size(iTests);
}

GpUnitTestHandlerStatistics GpUnitTestGroup::Run (GpUnitTestHandler& aHandler)
{
    iRunStartSTS    = GpDateTimeOps::SSteadyTS_us();
    iCurrentHandler = &aHandler;
    aHandler.OnTestGroupRunStart(*this);

    GpUnitTestHandlerStatistics statistics;

    statistics.startTs              = GpDateTimeOps::SUnixTS_ms();
    const microseconds_t startSTS   = GpDateTimeOps::SSteadyTS_us();

    GpRAIIonDestruct onReturn
    (
        [&]()
        {
            statistics.totalTime    = GpDateTimeOps::SSteadyTS_us() - startSTS;
            statistics.finishTs     = GpDateTimeOps::SUnixTS_ms();

            aHandler.OnTestGroupRunEnd(*this, statistics);
        }
    );

    if (StartSuite(aHandler) == false)
    {
        statistics.suiteStartFailedCount    = 1;
        statistics.skippedCount             = std::size(iTests);
        return statistics;
    }

    for (GpUnitTest::SP& testSP: iTests)
    {
        GpUnitTest& test = testSP.V();

        iRunStartSTS        = GpDateTimeOps::SSteadyTS_us();
        iCurrentTestName    = test.Name();
        iTestExpectFailsCnt = 0;

        try
        {
            aHandler.OnTestStart(iSuite->Name(), test.Name());

            testSP.V().Run(iSuite.V());

            if (iTestExpectFailsCnt == 0)
            {
                aHandler.OnTestPass
                (
                    iSuite->Name(),
                    test.Name(),
                    GpDateTimeOps::SSteadyTS_us() - iRunStartSTS
                );

                statistics.passedCount++;
            } else
            {
                statistics.failedCount++;
            }
        } catch (const GpUnitTestAssert& aTestAssert)
        {
            statistics.failedCount++;

            aHandler.OnTestFailedAssert
            (
                iSuite->Name(),
                test.Name(),
                aTestAssert.Message(),
                aTestAssert.SourceLocation(),
                GpDateTimeOps::SSteadyTS_us() - iRunStartSTS
            );

            break;
        } catch (const std::exception& aEx)
        {
            statistics.failedCount++;

            aHandler.OnTestException
            (
                iSuite->Name(),
                test.Name(),
                aEx,
                GpDateTimeOps::SSteadyTS_us() - iRunStartSTS
            );
        } catch (...)
        {
            statistics.failedCount++;

            aHandler.OnTestUnknownException
            (
                iSuite->Name(),
                test.Name(),
                GpDateTimeOps::SSteadyTS_us() - iRunStartSTS
            );
        }
    }

    statistics.skippedCount = std::size(iTests) - (statistics.failedCount + statistics.passedCount);

    if (StopSuite(aHandler) == false)
    {
        statistics.suiteStopFailedCount = 1;
    }

    if (iRuningGroupsCountOptRef.has_value())
    {
        iRuningGroupsCountOptRef.value().get()--;
    }

    return statistics;
}

bool    GpUnitTestGroup::StartSuite (GpUnitTestHandler& aHandler)
{
    if (iSuite.IsNULL())
    {
        return true;
    }

    std::string lastSuiteName;

    try
    {
        GpUnitTestSuiteGroup& suite = iSuite.V();

        lastSuiteName = suite.Name();
        suite.BeforeTests(aHandler, *this);

        return true;
    } catch (const std::exception& aEx)
    {
        aHandler.OnTestException
        (
            Suite()->Name(),
            "Start environment '"_sv + lastSuiteName + "'"_sv,
            aEx,
            GpDateTimeOps::SSteadyTS_us() - iRunStartSTS
        );
    } catch (...)
    {
        aHandler.OnTestUnknownException
        (
            Suite()->Name(),
            "Start environment '"_sv + lastSuiteName + "'"_sv,
            GpDateTimeOps::SSteadyTS_us() - iRunStartSTS
        );
    }

    GpUnitTestSuiteGroup& suite = iSuite.V();
    suite.AfterTests();

    return false;
}

bool    GpUnitTestGroup::StopSuite (GpUnitTestHandler& aHandler)
{
    if (iSuite.IsNULL())
    {
        return true;
    }

    std::string lastSuiteName;

    try
    {
        GpUnitTestSuiteGroup& suite = iSuite.V();
        lastSuiteName = suite.Name();
        suite.AfterTests();

        return true;
    } catch (const std::exception& aEx)
    {
        aHandler.OnTestException
        (
            Suite()->Name(),
            "Stop environment '"_sv + lastSuiteName + "'"_sv,
            aEx,
            GpDateTimeOps::SSteadyTS_us() - iRunStartSTS
        );
    } catch (...)
    {
        aHandler.OnTestUnknownException
        (
            Suite()->Name(),
            "Stop environment '"_sv + lastSuiteName + "'"_sv,
            GpDateTimeOps::SSteadyTS_us() - iRunStartSTS
        );
    }

    return false;
}

}// namespace GPlatform::UnitTest
