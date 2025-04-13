#include <GpUnitTests/AppService/GpUnitTestAppCfgDescFactory.hpp>
#include <GpUnitTests/AppService/GpUnitTestAppCfgDesc.hpp>

namespace GPlatform::UnitTest {

GpServiceCfgBaseDesc::SP    GpUnitTestAppCfgDescFactory::NewInstance (void) const
{
    return MakeSP<GpUnitTestAppCfgDesc>();
}

}// namespace GPlatform::UnitTest
