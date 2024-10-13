#include "GpUnitTestAppCmdArgsDescFactory.hpp"
#include "GpUnitTestAppCmdArgsDesc.hpp"

namespace GPlatform::UnitTest {

GpServiceArgBaseDescFactory::ResT   GpUnitTestAppCmdArgsDescFactory::NewInstance
(
    [[maybe_unused]] const size_t       aArgc,
    [[maybe_unused]] const char* const  aArgv[]
) const
{
    return {MakeSP<GpUnitTestAppCmdArgsDesc>(), false};
}

}// namespace GPlatform::UnitTest
