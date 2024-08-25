#pragma once

#include <GpService/GpServiceMainTaskFactory.hpp>
#include <GpUnitTests/GpUnitTests_globals.hpp>

namespace GPlatform::UnitTest {

class GP_UNIT_TESTS_API GpUnitTestAppMainTaskFactory final: public GpServiceMainTaskFactory
{
public:
   CLASS_REMOVE_CTRS_MOVE_COPY(GpUnitTestAppMainTaskFactory)
   CLASS_DD(GpUnitTestAppMainTaskFactory)

public:
                                   GpUnitTestAppMainTaskFactory     (void) noexcept;
   virtual                          ~GpUnitTestAppMainTaskFactory   (void) noexcept override final;

   virtual GpServiceMainTask::SP    NewInstance                     (const GpServiceArgBaseDesc& aServiceArgsDesc,
                                                                     const GpServiceCfgBaseDesc& aServiceCfgDesc) const override final;
};

}// namespace GPlatform::UnitTest
