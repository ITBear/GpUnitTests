#pragma once

#include <GpUnitTests/GpUnitTests_globals.hpp>

#include <GpCore2/GpUtils/Types/Units/Other/unix_ts_t.hpp>
#include <GpCore2/GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpMutex.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

namespace GPlatform::UnitTest {

class GP_UNIT_TESTS_API GpUnitTestHandlerStatistics
{
public:
                                        GpUnitTestHandlerStatistics (void) noexcept = default;
                                        GpUnitTestHandlerStatistics (const GpUnitTestHandlerStatistics& aStatistics) noexcept;
                                        GpUnitTestHandlerStatistics (GpUnitTestHandlerStatistics&& aStatistics) noexcept;

    static std::string                  SToString                   (const GpUnitTestHandlerStatistics& aStatistics);
    static void                         SSetSumm                    (GpUnitTestHandlerStatistics&       aStatisticsAOut,
                                                                     const GpUnitTestHandlerStatistics& aStatisticsB);
    static GpUnitTestHandlerStatistics  SSumm                       (const GpUnitTestHandlerStatistics& aStatisticsA,
                                                                     const GpUnitTestHandlerStatistics& aStatisticsB);

    GpUnitTestHandlerStatistics&        operator=                   (const GpUnitTestHandlerStatistics& aStatistics) noexcept;
    GpUnitTestHandlerStatistics&        operator=                   (GpUnitTestHandlerStatistics&& aStatistics) noexcept;

public:
    mutable GpMutex     iMutex;

    size_t              suiteStartFailedCount   = 0;
    size_t              suiteStopFailedCount    = 0;
    size_t              passedCount             = 0;
    size_t              failedCount             = 0;
    size_t              skippedCount            = 0;
    size_t              disabledCount           = 0;
    unix_ts_ms_t        startTs                 = 0_unix_ts_ms;
    unix_ts_ms_t        finishTs                = 0_unix_ts_ms;
    microseconds_t      totalTime               = 0.0_si_us;
};

}// namespace GPlatform::UnitTest
