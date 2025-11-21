#include <GpUnitTests/AppService/GpUnitTestAppMainTaskFactory.hpp>
#include <GpUnitTests/AppService/GpUnitTestAppMainTask.hpp>

namespace GPlatform::UnitTest {

GpUnitTestAppMainTaskFactory::GpUnitTestAppMainTaskFactory (void) noexcept
{
}

GpUnitTestAppMainTaskFactory::~GpUnitTestAppMainTaskFactory (void) noexcept
{
}

GpServiceMainTask::SP   GpUnitTestAppMainTaskFactory::NewInstance (void) const
{
    return MakeSP<GpUnitTestAppMainTask>();
}

}// namespace GPlatform::UnitTest
