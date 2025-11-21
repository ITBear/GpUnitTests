#include <GpUnitTests/AppService/GpUnitTestAppCmdArgsDesc.hpp>
#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>

namespace GPlatform::UnitTest {

REFLECT_IMPLEMENT(GpUnitTestAppCmdArgsDesc, GP_MODULE_UUID)

GpUnitTestAppCmdArgsDesc::GpUnitTestAppCmdArgsDesc (void) noexcept
{
}

GpUnitTestAppCmdArgsDesc::GpUnitTestAppCmdArgsDesc (const GpUnitTestAppCmdArgsDesc& aDesc):
GpServiceArgBaseDesc{aDesc},
unit_test_filter{aDesc.unit_test_filter}
{
}

GpUnitTestAppCmdArgsDesc::GpUnitTestAppCmdArgsDesc (GpUnitTestAppCmdArgsDesc&& aDesc) noexcept:
GpServiceArgBaseDesc{std::move(aDesc)},
unit_test_filter{std::move(aDesc.unit_test_filter)}
{
}

GpUnitTestAppCmdArgsDesc::~GpUnitTestAppCmdArgsDesc (void) noexcept
{
}

void    GpUnitTestAppCmdArgsDesc::_SReflectCollectProps (GpReflectProp::SmallVecVal& aPropsOut)
{
    PROP(unit_test_filter);
}

}// namespace GPlatform::UnitTest
