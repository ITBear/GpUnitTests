#include <GpUnitTests/AppService/GpUnitTestAppCfgDesc.hpp>
#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>

namespace GPlatform::UnitTest {

REFLECT_IMPLEMENT(GpUnitTestAppCfgDesc, GP_MODULE_UUID)

GpUnitTestAppCfgDesc::GpUnitTestAppCfgDesc (void) noexcept
{
}

GpUnitTestAppCfgDesc::GpUnitTestAppCfgDesc (const GpUnitTestAppCfgDesc& aDesc):
GpServiceCfgBaseDesc{aDesc}
{
}

GpUnitTestAppCfgDesc::GpUnitTestAppCfgDesc (GpUnitTestAppCfgDesc&& aDesc) noexcept:
GpServiceCfgBaseDesc{std::move(aDesc)}
{
}

GpUnitTestAppCfgDesc::~GpUnitTestAppCfgDesc (void) noexcept
{
}

void    GpUnitTestAppCfgDesc::_SReflectCollectProps (GpReflectProp::SmallVecVal& /*aPropsOut*/)
{
}

}// namespace GPlatform::UnitTest
