#include "GpUnitTestLogOutHandler.hpp"
#include "../GpUnitTestAssert.hpp"
#include "GpUnitTestGroup.hpp"

#include <GpLog/GpLogCore/GpLog.hpp>

namespace GPlatform::UnitTest {

GpUnitTestLogOutHandler::~GpUnitTestLogOutHandler   (void) noexcept
{   
}

void    GpUnitTestLogOutHandler::OnManagerStart (void)
{
    GpUnitTestLayeredHandler::OnManagerStart();
}

void    GpUnitTestLogOutHandler::OnManagerDone (const GpUnitTestHandlerStatistics&  aStatistics)
{
    std::string msg;
    msg.reserve(1024);

    msg.append("[========]: --------------------------- DONE ALL TESTS: ---------------------------\n"_sv);
    msg.append(GpUnitTestHandlerStatistics::SToString(aStatistics));

    if (aStatistics.failedCount == 0)
    {
        LOG_INFO(std::move(msg), iGuid);
    } else
    {
        LOG_ERROR(std::move(msg), iGuid);
    }

    LOG_END_CHAIN(iGuid);

    GpUnitTestLayeredHandler::OnManagerDone(aStatistics);
}

void    GpUnitTestLogOutHandler::OnTestGroupRunStart (const GpUnitTestGroup& aUnitTestGroup)
{
    std::string msg;
    msg.reserve(256);

    msg.append("[========]: Start new UNIT TEST group '"_sv).append(aUnitTestGroup.Suite()->Name()).append("' RUN..."_sv);
    msg.append("\n[========]: Total tests count: "_sv).append(std::to_string(std::size(aUnitTestGroup.Tests())));

    LOG_INFO(std::move(msg), iGuid);

    GpUnitTestLayeredHandler::OnTestGroupRunStart(aUnitTestGroup);
}

void    GpUnitTestLogOutHandler::OnTestGroupRunEnd
(
    const GpUnitTestGroup&              aUnitTestGroup,
    const GpUnitTestHandlerStatistics&  aStatistics
)
{
    std::string msg;
    msg.reserve(1024);

    msg.append("[========]: Done UNIT TEST group '"_sv).append(aUnitTestGroup.Suite()->Name()).append("' RUN...\n"_sv);
    msg.append(GpUnitTestHandlerStatistics::SToString(aStatistics));

    if (aStatistics.failedCount == 0)
    {
        LOG_INFO(std::move(msg), iGuid);
    } else
    {
        LOG_ERROR(std::move(msg), iGuid);
    }

    LOG_END_CHAIN(iGuid);

    GpUnitTestLayeredHandler::OnTestGroupRunEnd(aUnitTestGroup, aStatistics);
}

void    GpUnitTestLogOutHandler::OnTestStart
(
    std::string_view    aTestSuitName,
    std::string_view    aTestName
)
{
    GpUnitTestLayeredHandler::OnTestStart(aTestSuitName, aTestName);
}

void    GpUnitTestLogOutHandler::OnTestPass
(
    std::string_view        aTestSuitName,
    std::string_view        aTestName,
    const microseconds_t    aTotalTime
)
{
    std::string msg;
    msg.reserve(180);

    msg.append("[  PASS  ]: Run test \""_sv).append(aTestSuitName).append(". "_sv).append(aTestName).append("\"... ("_sv)
       .append(std::to_string(seconds_t(aTotalTime).Value())).append("s.)"_sv);

    LOG_INFO(std::move(msg), iGuid);

    GpUnitTestLayeredHandler::OnTestPass(aTestSuitName, aTestName, aTotalTime);
}

void    GpUnitTestLogOutHandler::OnTestFailedExpect
(
    std::string_view        aTestSuitName,
    std::string_view        aTestName,
    std::string_view        aMsg,
    const SourceLocationT&  aLocation,
    const microseconds_t    aTotalTime
)
{
    std::string msg;
    msg.reserve(1024);

    msg.append("[ FAILED ]: EXPECT Run test \""_sv).append(aTestSuitName).append(". "_sv).append(aTestName).append("\"... ("_sv)
       .append(std::to_string(seconds_t(aTotalTime).Value())).append("s.)"_sv);

    msg.append("\n[        ]: REASON: ").append(aMsg);
    msg.append("\n[        ]: in file '"_sv).append(aLocation.file_name()).append("'"_sv);
    msg.append("\n[        ]: function '"_sv).append(aLocation.function_name()).append("'"_sv);
    msg.append("\n[        ]: line "_sv).append(std::to_string(aLocation.line()));
    msg.append("\n[        ]: column "_sv).append(std::to_string(aLocation.column()));

    LOG_ERROR(std::move(msg), iGuid);

    GpUnitTestLayeredHandler::OnTestFailedExpect(aTestSuitName, aTestName, aMsg, aLocation, aTotalTime);
}

void    GpUnitTestLogOutHandler::OnTestFailedAssert
(
    std::string_view        aTestSuitName,
    std::string_view        aTestName,
    std::string_view        aMsg,
    const SourceLocationT&  aLocation,
    const microseconds_t    aTotalTime
)
{
    std::string msg;
    msg.reserve(1024);

    msg.append("[ FAILED ]: ASSERT Run test \""_sv).append(aTestSuitName).append(". "_sv).append(aTestName).append("\"... ("_sv)
       .append(std::to_string(seconds_t(aTotalTime).Value())).append("s.)"_sv);

    msg.append("\n[        ]: REASON: ").append(aMsg);
    msg.append("\n[        ]: in file '"_sv).append(aLocation.file_name()).append("'"_sv);
    msg.append("\n[        ]: function '"_sv).append(aLocation.function_name()).append("'"_sv);
    msg.append("\n[        ]: line "_sv).append(std::to_string(aLocation.line()));
    msg.append("\n[        ]: column "_sv).append(std::to_string(aLocation.column()));

    LOG_ERROR(std::move(msg), iGuid);

    GpUnitTestLayeredHandler::OnTestFailedAssert(aTestSuitName, aTestName, aMsg, aLocation, aTotalTime);
}

void    GpUnitTestLogOutHandler::OnTestException
(
    std::string_view        aTestSuitName,
    std::string_view        aTestName,
    const std::exception&   aEx,
    const microseconds_t    aTotalTime
)
{
    std::string msg;
    msg.reserve(512);

    msg.append("[ FAILED ]: Run test \""_sv).append(aTestSuitName).append(". "_sv).append(aTestName).append("\"... ("_sv)
       .append(std::to_string(seconds_t(aTotalTime).Value())).append("s.)"_sv);

    msg.append("\n[        ]: REASON: exception was caught");
    msg.append("\n[        ]: MESSAGE: "_sv).append(aEx.what());

    LOG_ERROR(std::move(msg), iGuid);

    GpUnitTestLayeredHandler::OnTestException(aTestSuitName, aTestName, aEx, aTotalTime);
}

void    GpUnitTestLogOutHandler::OnTestUnknownException
(
    std::string_view        aTestSuitName,
    std::string_view        aTestName,
    const microseconds_t    aTotalTime
)
{
    std::string msg;
    msg.reserve(512);

    msg.append("[ FAILED ]: Run test \""_sv).append(aTestSuitName).append(". "_sv).append(aTestName).append("\"... ("_sv)
       .append(std::to_string(seconds_t(aTotalTime).Value())).append("s.)"_sv);

    msg.append("\n[        ]: REASON: UNKNOWN exception was caught");

    LOG_ERROR(std::move(msg), iGuid);

    GpUnitTestLayeredHandler::OnTestUnknownException(aTestSuitName, aTestName, aTotalTime);
}

}// namespace GPlatform::UnitTest
