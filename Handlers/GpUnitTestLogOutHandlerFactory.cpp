#include <GpUnitTests/Handlers/GpUnitTestLogOutHandlerFactory.hpp>
#include <GpUnitTests/Handlers/GpUnitTestLogOutHandler.hpp>

namespace GPlatform::UnitTest {

GpSP<GpUnitTestHandler> GpUnitTestLogOutHandlerFactory::NewInstance (void) noexcept
{
    return MakeSP<GpUnitTestLogOutHandler>();
}

}// namespace GPlatform::UnitTest
