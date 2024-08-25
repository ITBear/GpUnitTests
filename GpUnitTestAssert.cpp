#include <GpUnitTests/GpUnitTestAssert.hpp>

namespace GPlatform::UnitTest {

GpUnitTestAssert::GpUnitTestAssert
(
    std::string_view        aMsg,
    const SourceLocationT&  aLocation
) noexcept:
GpException{aMsg, aLocation}
{
}

GpUnitTestAssert::~GpUnitTestAssert (void) noexcept
{
}

}// namespace GPlatform::UnitTest
