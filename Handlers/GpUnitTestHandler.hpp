#pragma once

#include <GpUnitTests/Handlers/GpUnitTestHandlerStatistics.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>

namespace GPlatform::UnitTest {

class GpUnitTestGroup;

class GpUnitTestHandler
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpUnitTestHandler)
    CLASS_DD(GpUnitTestHandler)

public:
                        GpUnitTestHandler       (void) noexcept = default;
    virtual             ~GpUnitTestHandler      (void) noexcept = default;

    // Manager
    virtual void        OnManagerStart          (void) = 0;
    virtual void        OnManagerDone           (const GpUnitTestHandlerStatistics& aStatistics) = 0;

    // Tests group
    virtual void        OnTestGroupRunStart     (const GpUnitTestGroup& aUnitTestGroup) = 0;
    virtual void        OnTestGroupRunEnd       (const GpUnitTestGroup&             aUnitTestGroup,
                                                 const GpUnitTestHandlerStatistics& aStatistics) = 0;

    // Unit test
    virtual void        OnTestStart             (std::string_view       aTestSuitName,
                                                 std::string_view       aTestName) = 0;
    virtual void        OnTestPass              (std::string_view       aTestSuitName,
                                                 std::string_view       aTestName,
                                                 const microseconds_t   aTotalTime) = 0;
    virtual void        OnTestFailedExpect      (std::string_view       aTestSuitName,
                                                 std::string_view       aTestName,
                                                 std::string_view       aMsg,
                                                 const SourceLocationT& aLocation,
                                                 const microseconds_t   aTotalTime) = 0;
    virtual void        OnTestFailedAssert      (std::string_view       aTestSuitName,
                                                 std::string_view       aTestName,
                                                 std::string_view       aMsg,
                                                 const SourceLocationT& aLocation,
                                                 const microseconds_t   aTotalTime) = 0;
    virtual void        OnTestException         (std::string_view       aTestSuitName,
                                                 std::string_view       aTestName,
                                                 const std::exception&  aEx,
                                                 const microseconds_t   aTotalTime) = 0;
    virtual void        OnTestUnknownException  (std::string_view       aTestSuitName,
                                                 std::string_view       aTestName,
                                                 const microseconds_t   aTotalTime) = 0;
};

}// namespace GPlatform::UnitTest
