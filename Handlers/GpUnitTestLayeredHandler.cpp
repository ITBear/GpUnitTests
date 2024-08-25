#include "GpUnitTestLayeredHandler.hpp"

namespace GPlatform::UnitTest {

GpUnitTestLayeredHandler::~GpUnitTestLayeredHandler (void) noexcept
{
}

void    GpUnitTestLayeredHandler::OnManagerStart (void)
{
    if (iSublayerHandler.IsNotNULL())
    {
        iSublayerHandler.Vn().OnManagerStart();
    }
}

void    GpUnitTestLayeredHandler::OnManagerDone (const GpUnitTestHandlerStatistics& aStatistics)
{
    if (iSublayerHandler.IsNotNULL())
    {
        iSublayerHandler.Vn().OnManagerDone(aStatistics);
    }
}

void    GpUnitTestLayeredHandler::OnTestGroupRunStart (const GpUnitTestGroup& aUnitTestGroup)
{
    if (iSublayerHandler.IsNotNULL())
    {
        iSublayerHandler.Vn().OnTestGroupRunStart(aUnitTestGroup);
    }
}

void    GpUnitTestLayeredHandler::OnTestGroupRunEnd
(
    const GpUnitTestGroup&              aUnitTestGroup,
    const GpUnitTestHandlerStatistics&  aStatistics
)
{
    if (iSublayerHandler.IsNotNULL())
    {
        iSublayerHandler.Vn().OnTestGroupRunEnd(aUnitTestGroup, aStatistics);
    }
}

void    GpUnitTestLayeredHandler::OnTestStart
(
    std::string_view    aTestSuitName,
    std::string_view    aTestName
)
{
    if (iSublayerHandler.IsNotNULL())
    {
        iSublayerHandler.Vn().OnTestStart(aTestSuitName, aTestName);
    }
}

void    GpUnitTestLayeredHandler::OnTestPass
(
    std::string_view        aTestSuitName,
    std::string_view        aTestName,
    const microseconds_t    aTotalTime
)
{
    if (iSublayerHandler.IsNotNULL())
    {
        iSublayerHandler.Vn().OnTestPass(aTestSuitName, aTestName, aTotalTime);
    }
}

void    GpUnitTestLayeredHandler::OnTestFailedExpect
(
    std::string_view        aTestSuitName,
    std::string_view        aTestName,
    std::string_view        aMsg,
    const SourceLocationT&  aLocation,
    const microseconds_t    aTotalTime
)
{
    if (iSublayerHandler.IsNotNULL())
    {
        iSublayerHandler.Vn().OnTestFailedExpect(aTestSuitName, aTestName, aMsg, aLocation, aTotalTime);
    }
}

void    GpUnitTestLayeredHandler::OnTestFailedAssert
(
    std::string_view        aTestSuitName,
    std::string_view        aTestName,
    std::string_view        aMsg,
    const SourceLocationT&  aLocation,
    const microseconds_t    aTotalTime
)
{
    if (iSublayerHandler.IsNotNULL())
    {
        iSublayerHandler.Vn().OnTestFailedAssert(aTestSuitName, aTestName, aMsg, aLocation, aTotalTime);
    }
}

void    GpUnitTestLayeredHandler::OnTestException
(
    std::string_view        aTestSuitName,
    std::string_view        aTestName,
    const std::exception&   aEx,
    const microseconds_t    aTotalTime
)
{
    if (iSublayerHandler.IsNotNULL())
    {
        iSublayerHandler.Vn().OnTestException(aTestSuitName, aTestName, aEx, aTotalTime);
    }
}

void    GpUnitTestLayeredHandler::OnTestUnknownException
(
    std::string_view        aTestSuitName,
    std::string_view        aTestName,
    const microseconds_t    aTotalTime
)
{
    if (iSublayerHandler.IsNotNULL())
    {
        iSublayerHandler.Vn().OnTestUnknownException(aTestSuitName, aTestName, aTotalTime);
    }
}

}// namespace GPlatform::UnitTest
