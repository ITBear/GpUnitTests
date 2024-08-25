#pragma once

#include "../GpUnitTests_globals.hpp"
#include "GpUnitTestHandlerFactory.hpp"

namespace GPlatform::UnitTest {

class GP_UNIT_TESTS_API GpUnitTestLogOutHandlerFactory final: public GpUnitTestHandlerFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpUnitTestLogOutHandlerFactory)
    CLASS_DD(GpUnitTestLogOutHandlerFactory)

public:
                                    GpUnitTestLogOutHandlerFactory  (void) noexcept = default;
    virtual                         ~GpUnitTestLogOutHandlerFactory (void) noexcept override final = default;

    virtual GpSP<GpUnitTestHandler> NewInstance                     (void) noexcept override final;
};

}// namespace GPlatform::UnitTest
