#include "GpUnitTestAppCfgDesc.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpGlobalStructCatalogC.hpp>

namespace GPlatform::UnitTest {

REFLECT_IMPLEMENT(GpUnitTestAppCfgDesc, GP_MODULE_UUID)

constexpr std::string_view __serviceCfgDesc_reg_name = "GpUnitTestApp/service_cfg"_sv;

GpUnitTestAppCfgDesc::GpUnitTestAppCfgDesc (void) noexcept
{
}

GpUnitTestAppCfgDesc::GpUnitTestAppCfgDesc (const GpUnitTestAppCfgDesc& aDesc):
GpServiceCfgBaseDesc(aDesc)
{
}

GpUnitTestAppCfgDesc::GpUnitTestAppCfgDesc (GpUnitTestAppCfgDesc&& aDesc) noexcept:
GpServiceCfgBaseDesc(std::move(aDesc))
{
}

GpUnitTestAppCfgDesc::~GpUnitTestAppCfgDesc (void) noexcept
{
}

void    GpUnitTestAppCfgDesc::_SReflectCollectProps (GpReflectProp::SmallVecVal& /*aPropsOut*/)
{
}

const GpUnitTestAppCfgDesc& GpUnitTestAppCfgDesc::SGet (void)
{
    const GpUnitTestAppCfgDesc& serviceCfgDesc = GpGlobalStructCatalogC::S().FindAs<std::reference_wrapper<const GpUnitTestAppCfgDesc>>
    (
        __serviceCfgDesc_reg_name
    ).get();

    return serviceCfgDesc;
}

void    GpUnitTestAppCfgDesc::SRegister (const GpUnitTestAppCfgDesc& aServiceCfgDesc)
{
    GpGlobalStructCatalogC::S().Register
    (
        std::string(__serviceCfgDesc_reg_name),
        std::reference_wrapper(aServiceCfgDesc)
    );
}

}// namespace GPlatform::UnitTest
