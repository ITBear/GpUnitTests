#include "GpUnitTestLogOutHandlerFactory.hpp"
#include "GpUnitTestLogOutHandler.hpp"

namespace GPlatform::UnitTest {

GpSP<GpUnitTestHandler> GpUnitTestLogOutHandlerFactory::NewInstance (void) noexcept
{
    return MakeSP<GpUnitTestLogOutHandler>();
}

}// namespace GPlatform::UnitTest
