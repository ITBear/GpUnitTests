#pragma once

#include <GpService/ArgParser/GpServiceArgBaseDescFactory.hpp>
#include "../GpUnitTests_globals.hpp"

namespace GPlatform::UnitTest {

class GP_UNIT_TESTS_API GpUnitTestAppCmdArgsDescFactory final: public GpServiceArgBaseDescFactory
{
public:
    CLASS_DD(GpUnitTestAppCmdArgsDescFactory)

public:
                    GpUnitTestAppCmdArgsDescFactory     (void) noexcept = default;
    virtual         ~GpUnitTestAppCmdArgsDescFactory    (void) noexcept override final = default;

public:
    virtual ResT    NewInstance                         (size_t             aArgc,
                                                         const char* const  aArgv[]) const override final;
};

}// namespace GPlatform::UnitTest
