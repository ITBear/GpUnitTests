#include <GpUnitTests/Handlers/GpUnitTestLogOutHandlerFactory.hpp>
#include <GpUnitTests/Handlers/GpUnitTestLogOutHandler.hpp>

namespace GPlatform::UnitTest {

GpUnitTestHandler::UP   GpUnitTestLogOutHandlerFactory::NewInstance (void) noexcept
{
    return std::make_unique<GpUnitTestLogOutHandler>();
}

}// namespace GPlatform::UnitTest
