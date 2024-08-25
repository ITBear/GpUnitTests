#include "GpUnitTestAppCfgDescFactory.hpp"
#include "GpUnitTestAppCfgDesc.hpp"

namespace GPlatform::UnitTest {

GpServiceCfgBaseDesc::SP    GpUnitTestAppCfgDescFactory::NewInstance (void) const
{
    return MakeSP<GpUnitTestAppCfgDesc>();
}

}// namespace GPlatform::UnitTest
