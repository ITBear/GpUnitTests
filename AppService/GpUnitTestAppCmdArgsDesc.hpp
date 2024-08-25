#pragma once

#include <GpService/ArgParser/GpServiceArgBaseDesc.hpp>
#include "../GpUnitTests_globals.hpp"

namespace GPlatform::UnitTest {

class GP_UNIT_TESTS_API GpUnitTestAppCmdArgsDesc final: public GpServiceArgBaseDesc
{
public:
    CLASS_DD(GpUnitTestAppCmdArgsDesc)
    REFLECT_DECLARE("4ce479f0-6363-4463-518a-173903f3eafc"_uuid)

public:
                                            GpUnitTestAppCmdArgsDesc    (void) noexcept;
                                            GpUnitTestAppCmdArgsDesc    (const GpUnitTestAppCmdArgsDesc& aDesc);
                                            GpUnitTestAppCmdArgsDesc    (GpUnitTestAppCmdArgsDesc&& aDesc) noexcept;
    virtual                                 ~GpUnitTestAppCmdArgsDesc   (void) noexcept override final;

    static const GpUnitTestAppCmdArgsDesc&  SGet                        (void);
    static void                             SRegister                   (const GpUnitTestAppCmdArgsDesc& aCmdArgsDesc);

public:
    std::string                             filter;
};

}// namespace GPlatform::UnitTest
