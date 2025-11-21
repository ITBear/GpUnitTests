#include <GpUnitTests/GpUnitTestsLib.hpp>
#include <GpCore2/GpUtils/Other/GpLinkedLibsInfo.hpp>

GP_STATIC_INITIALIZER_IMPL(GpUnitTests)
GP_LIB_REGISTRATOR(GpUnitTestsLib)

void    GpUnitTests_StaticInitializer::OnInitialize (void)
{
    GpUnitTestsLib::SRegisterSelf();
}
