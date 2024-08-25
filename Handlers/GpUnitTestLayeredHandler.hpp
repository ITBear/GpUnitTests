#pragma once

#include "../GpUnitTests_globals.hpp"
#include "GpUnitTestHandler.hpp"

namespace GPlatform::UnitTest {

class GP_UNIT_TESTS_API GpUnitTestLayeredHandler: public GpUnitTestHandler
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpUnitTestLayeredHandler)
    CLASS_DD(GpUnitTestLayeredHandler)

public:
    inline                  GpUnitTestLayeredHandler    (void) noexcept;
    inline                  GpUnitTestLayeredHandler    (GpUnitTestHandler::SP aSublayerHandler) noexcept;
    virtual                 ~GpUnitTestLayeredHandler   (void) noexcept override;

    virtual void            OnManagerStart              (void) override;
    virtual void            OnManagerDone               (const GpUnitTestHandlerStatistics& aStatistics) override;

    virtual void            OnTestGroupRunStart         (const GpUnitTestGroup& aUnitTestGroup) override;
    virtual void            OnTestGroupRunEnd           (const GpUnitTestGroup&             aUnitTestGroup,
                                                         const GpUnitTestHandlerStatistics& aStatistics) override;

    virtual void            OnTestStart                 (std::string_view       aTestSuitName,
                                                         std::string_view       aTestName) override;
    virtual void            OnTestPass                  (std::string_view       aTestSuitName,
                                                         std::string_view       aTestName,
                                                         const microseconds_t   aTotalTime) override;
    virtual void            OnTestFailedExpect          (std::string_view       aTestSuitName,
                                                         std::string_view       aTestName,
                                                         std::string_view       aMsg,
                                                         const SourceLocationT& aLocation,
                                                         const microseconds_t   aTotalTime) override;
    virtual void            OnTestFailedAssert          (std::string_view       aTestSuitName,
                                                         std::string_view       aTestName,
                                                         std::string_view       aMsg,
                                                         const SourceLocationT& aLocation,
                                                         const microseconds_t   aTotalTime) override;
    virtual void            OnTestException             (std::string_view       aTestSuitName,
                                                         std::string_view       aTestName,
                                                         const std::exception&  aEx,
                                                         const microseconds_t   aTotalTime) override;
    virtual void            OnTestUnknownException      (std::string_view       aTestSuitName,
                                                         std::string_view       aTestName,
                                                         const microseconds_t   aTotalTime) override;

private:
    GpUnitTestHandler::SP   iSublayerHandler;
};

GpUnitTestLayeredHandler::GpUnitTestLayeredHandler (void) noexcept
{
}

GpUnitTestLayeredHandler::GpUnitTestLayeredHandler (GpUnitTestHandler::SP aSublayerHandler) noexcept:
iSublayerHandler(std::move(aSublayerHandler))
{
}

}// namespace GPlatform::UnitTest
