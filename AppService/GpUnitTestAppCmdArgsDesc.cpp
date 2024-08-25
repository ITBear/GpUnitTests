#include "GpUnitTestAppCmdArgsDesc.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpGlobalStructCatalogC.hpp>

namespace GPlatform::UnitTest {

REFLECT_IMPLEMENT(GpUnitTestAppCmdArgsDesc, GP_MODULE_UUID)

constexpr std::string_view __serviceCmdArgsDesc_reg_name = "GpUnitTestApp/service_cmd_args"_sv;

GpUnitTestAppCmdArgsDesc::GpUnitTestAppCmdArgsDesc (void) noexcept
{
}

GpUnitTestAppCmdArgsDesc::GpUnitTestAppCmdArgsDesc (const GpUnitTestAppCmdArgsDesc& aDesc):
GpServiceArgBaseDesc(aDesc),
filter(aDesc.filter)
{
}

GpUnitTestAppCmdArgsDesc::GpUnitTestAppCmdArgsDesc (GpUnitTestAppCmdArgsDesc&& aDesc) noexcept:
GpServiceArgBaseDesc(std::move(aDesc)),
filter(std::move(aDesc.filter))
{
}

GpUnitTestAppCmdArgsDesc::~GpUnitTestAppCmdArgsDesc (void) noexcept
{
}

void    GpUnitTestAppCmdArgsDesc::_SReflectCollectProps (GpReflectProp::SmallVecVal& aPropsOut)
{
    PROP(filter);
}

const GpUnitTestAppCmdArgsDesc& GpUnitTestAppCmdArgsDesc::SGet (void)
{
    const GpUnitTestAppCmdArgsDesc& cmdArgsDesc = GpGlobalStructCatalogC::S().FindAs<std::reference_wrapper<const GpUnitTestAppCmdArgsDesc>>
    (
        __serviceCmdArgsDesc_reg_name
    ).get();

    return cmdArgsDesc;
}

void    GpUnitTestAppCmdArgsDesc::SRegister (const GpUnitTestAppCmdArgsDesc& aCmdArgsDesc)
{
    GpGlobalStructCatalogC::S().Register
    (
        std::string(__serviceCmdArgsDesc_reg_name),
        std::reference_wrapper(aCmdArgsDesc)
    );
}

}// namespace GPlatform::UnitTest
