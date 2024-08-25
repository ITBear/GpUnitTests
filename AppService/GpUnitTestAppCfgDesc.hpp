#pragma once

#include <GpService/Config/GpServiceCfgBaseDesc.hpp>
#include "../GpUnitTests_globals.hpp"

namespace GPlatform::UnitTest {

class GP_UNIT_TESTS_API GpUnitTestAppCfgDesc final: public GpServiceCfgBaseDesc
{
public:
    CLASS_DD(GpUnitTestAppCfgDesc)
    REFLECT_DECLARE("55680660-e027-4402-737d-087e81879b40"_uuid)

public:
                                        GpUnitTestAppCfgDesc    (void) noexcept;
                                        GpUnitTestAppCfgDesc    (const GpUnitTestAppCfgDesc& aDesc);
                                        GpUnitTestAppCfgDesc    (GpUnitTestAppCfgDesc&& aDesc) noexcept;
    virtual                             ~GpUnitTestAppCfgDesc   (void) noexcept override final;

    static const GpUnitTestAppCfgDesc&  SGet                    (void);
    static void                         SRegister               (const GpUnitTestAppCfgDesc& aServiceCfgDesc);

public:
};

}// namespace GPlatform::UnitTest
