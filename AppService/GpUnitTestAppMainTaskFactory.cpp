#include <GpUnitTests/AppService/GpUnitTestAppMainTaskFactory.hpp>
#include <GpUnitTests/AppService/GpUnitTestAppMainTask.hpp>

namespace GPlatform::UnitTest {

GpUnitTestAppMainTaskFactory::GpUnitTestAppMainTaskFactory (void) noexcept
{
}

GpUnitTestAppMainTaskFactory::~GpUnitTestAppMainTaskFactory (void) noexcept
{
}

GpServiceMainTask::SP   GpUnitTestAppMainTaskFactory::NewInstance
(
    const GpServiceArgBaseDesc& aServiceArgsDesc,
    const GpServiceCfgBaseDesc& aServiceCfgDesc
) const
{
    return MakeSP<GpUnitTestAppMainTask>
    (
        aServiceArgsDesc,
        aServiceCfgDesc
    );
}

}// namespace GPlatform::UnitTest
