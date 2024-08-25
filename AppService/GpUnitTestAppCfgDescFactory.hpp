#pragma once

#include <GpService/Config/GpServiceCfgBaseDescFactory.hpp>
#include "../GpUnitTests_globals.hpp"

namespace GPlatform::UnitTest {

class GP_UNIT_TESTS_API GpUnitTestAppCfgDescFactory final: public GpServiceCfgBaseDescFactory
{
public:
    CLASS_DD(GpUnitTestAppCfgDescFactory)

public:
                                        GpUnitTestAppCfgDescFactory     (void) noexcept = default;
    virtual                             ~GpUnitTestAppCfgDescFactory    (void) noexcept override final = default;

    virtual GpServiceCfgBaseDesc::SP    NewInstance                     (void) const override final;
};

}// namespace GPlatform::UnitTest
