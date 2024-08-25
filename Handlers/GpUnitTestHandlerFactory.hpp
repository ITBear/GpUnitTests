#pragma once

#include <GpUnitTests/Handlers/GpUnitTestHandler.hpp>

namespace GPlatform::UnitTest {

class GpUnitTestHandlerFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpUnitTestHandlerFactory)
    CLASS_DD(GpUnitTestHandlerFactory)

public:
                                    GpUnitTestHandlerFactory    (void) noexcept = default;
    virtual                         ~GpUnitTestHandlerFactory   (void) noexcept = default;

    virtual GpUnitTestHandler::SP   NewInstance                 (void) noexcept = 0;
};

}// namespace GPlatform::UnitTest
