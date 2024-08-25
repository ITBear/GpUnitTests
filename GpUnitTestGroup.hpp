#pragma once

#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

#include <GpUnitTests/GpUnitTests_globals.hpp>
#include <GpUnitTests/GpUnitTest.hpp>
#include <GpUnitTests/GpUnitTestSuiteGroup.hpp>
#include <GpUnitTests/Handlers/GpUnitTestHandlerStatistics.hpp>

namespace GPlatform::UnitTest {

class GpUnitTestHandler;

GP_ENUM(GP_UNIT_TESTS_API, GpUnitTestGroupRunMode,
    RUN_EXCLUSIVE,
    RUN_PARALLEL
);

class GP_UNIT_TESTS_API GpUnitTestGroup
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpUnitTestGroup)
    CLASS_DD(GpUnitTestGroup)

    using RuningGroupsCountT = std::optional<std::reference_wrapper<std::atomic_size_t>>;

public:
                                    GpUnitTestGroup     (GpUnitTestSuiteGroup::SP aSuite) noexcept;
                                    ~GpUnitTestGroup    (void) noexcept = default;

    void                            OnTestFailedExpect  (std::string_view       aMsg,
                                                         const SourceLocationT& aLocation);

    const GpUnitTest::C::Vec::SP&   Tests               (void) const noexcept {return iTests;}
    const GpUnitTestSuiteGroup::SP& Suite               (void) const noexcept {return iSuite;}
    GpUnitTestGroupRunMode::EnumT   RunMode             (void) const noexcept {return iRunMode;}
    void                            SetRunCounterAndInc (std::atomic_size_t& aRuningGroupsCountRef) noexcept;
    void                            AddTest             (GpUnitTest::SP aTest) {iTests.emplace_back(std::move(aTest));}
    size_t                          FilterTests         (std::string_view aFilter);
    GpUnitTestHandlerStatistics     Run                 (GpUnitTestHandler& aHandler);

private:
    bool                            StartSuite          (GpUnitTestHandler& aHandler);
    bool                            StopSuite           (GpUnitTestHandler& aHandler);

private:
    GpUnitTest::C::Vec::SP          iTests;
    size_t                          iTestExpectFailsCnt = 0;
    GpUnitTestHandler*              iCurrentHandler     = nullptr;
    std::string                     iCurrentTestName;
    microseconds_t                  iRunStartSTS;
    GpUnitTestSuiteGroup::SP        iSuite;
    GpUnitTestGroupRunMode::EnumT   iRunMode            = GpUnitTestGroupRunMode::RUN_EXCLUSIVE;
    RuningGroupsCountT              iRuningGroupsCountOptRef;
};

}// namespace GPlatform::UnitTest
