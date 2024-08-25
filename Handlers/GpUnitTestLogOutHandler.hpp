#pragma once

#include "GpUnitTestLayeredHandler.hpp"

#include <GpCore2/GpUtils/Types/UIDs/GpUUID.hpp>

namespace GPlatform::UnitTest {

class GP_UNIT_TESTS_API GpUnitTestLogOutHandler final: public GpUnitTestLayeredHandler
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpUnitTestLogOutHandler)
    CLASS_DD(GpUnitTestLogOutHandler)

public:
    inline              GpUnitTestLogOutHandler     (void);
    inline              GpUnitTestLogOutHandler     (GpUnitTestHandler::SP aSublayerHandler);
    virtual             ~GpUnitTestLogOutHandler    (void) noexcept override final;

    virtual void        OnManagerStart              (void) override;
    virtual void        OnManagerDone               (const GpUnitTestHandlerStatistics& aStatistics) override;

    virtual void        OnTestGroupRunStart         (const GpUnitTestGroup& aUnitTestGroup) override final;
    virtual void        OnTestGroupRunEnd           (const GpUnitTestGroup&             aUnitTestGroup,
                                                     const GpUnitTestHandlerStatistics& aStatistics) override final;

    virtual void        OnTestStart                 (std::string_view           aTestGroupName,
                                                     std::string_view           aTestName) override final;
    virtual void        OnTestPass                  (std::string_view           aTestGroupName,
                                                     std::string_view           aTestName,
                                                     const microseconds_t       aTotalTime) override final;
    virtual void        OnTestFailedExpect          (std::string_view           aTestGroupName,
                                                     std::string_view           aTestName,
                                                     std::string_view           aMsg,
                                                     const SourceLocationT&     aLocation,
                                                     const microseconds_t       aTotalTime) override;
    virtual void        OnTestFailedAssert          (std::string_view           aTestGroupName,
                                                     std::string_view           aTestName,
                                                     std::string_view           aMsg,
                                                     const SourceLocationT&     aLocation,
                                                     const microseconds_t       aTotalTime) override;
    virtual void        OnTestException             (std::string_view           aTestGroupName,
                                                     std::string_view           aTestName,
                                                     const std::exception&      aEx,
                                                     const microseconds_t       aTotalTime) override final;
    virtual void        OnTestUnknownException      (std::string_view           aTestGroupName,
                                                     std::string_view           aTestName,
                                                     const microseconds_t       aTotalTime) override final;

private:
    const GpUUID        iGuid;
};

GpUnitTestLogOutHandler::GpUnitTestLogOutHandler (void):
iGuid(GpUUID::SGenRandomV7())
{
}

GpUnitTestLogOutHandler::GpUnitTestLogOutHandler (GpUnitTestHandler::SP aSublayerHandler):
GpUnitTestLayeredHandler(aSublayerHandler),
iGuid(GpUUID::SGenRandomV7())
{
}

}// namespace GPlatform::UnitTest
