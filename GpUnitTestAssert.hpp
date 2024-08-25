#pragma once

#include <GpUnitTests/GpUnitTests_globals.hpp>
#include <GpCore2/GpUtils/Exceptions/GpException.hpp>

namespace GPlatform::UnitTest {

class GP_UNIT_TESTS_API GpUnitTestAssert final: public GpException
{
public:
                    GpUnitTestAssert    (void) noexcept = delete;
                    GpUnitTestAssert    (std::string_view       aMsg,
                                         const SourceLocationT& aLocation) noexcept;
    virtual         ~GpUnitTestAssert   (void) noexcept override final;
};

}// namespace GPlatform::UnitTest
