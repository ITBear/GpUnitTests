#pragma once

#include <GpUnitTests/GpUnitTests_globals.hpp>
#include <GpService/GpServiceMainTask.hpp>

namespace GPlatform::UnitTest {

class GP_UNIT_TESTS_API GpUnitTestAppMainTask final: public GpServiceMainTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpUnitTestAppMainTask)
    CLASS_DD(GpUnitTestAppMainTask)

public:
                                GpUnitTestAppMainTask   (const GpServiceArgBaseDesc& aServiceArgsDesc,
                                                         const GpServiceCfgBaseDesc& aServiceCfgDesc) noexcept;
    virtual                     ~GpUnitTestAppMainTask  (void) noexcept override final;

protected:
    virtual void                OnStart                 (void) override final;
    virtual GpTaskRunRes::EnumT OnStep                  (void) override final;
    virtual void                OnStop                  (StopExceptionsT& aStopExceptionsOut) noexcept override final;
    virtual void                OnStopException         (const GpException& aException) noexcept override final;
};

}// namespace GPlatform::UnitTest
