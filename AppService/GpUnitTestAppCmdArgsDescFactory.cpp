#include "GpUnitTestAppCmdArgsDescFactory.hpp"
#include "GpUnitTestAppCmdArgsDesc.hpp"

namespace GPlatform::UnitTest {

GpServiceArgBaseDesc::SP    GpUnitTestAppCmdArgsDescFactory::NewInstance
(
    const size_t    /*aArgc*/,
    char**          /*aArgv*/
) const
{
    return MakeSP<GpUnitTestAppCmdArgsDesc>();
}

}// namespace GPlatform::UnitTest
