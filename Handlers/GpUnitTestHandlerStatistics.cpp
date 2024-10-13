#include <GpUnitTests/Handlers/GpUnitTestHandlerStatistics.hpp>

#include <GpCore2/GpUtils/Types/Strings/GpStringLiterals.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/DateTime/GpDateTimeOps.hpp>

namespace GPlatform::UnitTest {

GpUnitTestHandlerStatistics::GpUnitTestHandlerStatistics (const GpUnitTestHandlerStatistics& aStatistics) noexcept:
suiteStartFailedCount(aStatistics.suiteStartFailedCount),
suiteStopFailedCount (aStatistics.suiteStopFailedCount ),
passedCount          (aStatistics.passedCount        ),
failedCount          (aStatistics.failedCount        ),
skippedCount         (aStatistics.skippedCount       ),
disabledCount        (aStatistics.disabledCount      ),
startTs              (aStatistics.startTs            ),
finishTs             (aStatistics.finishTs           ),
totalTime            (aStatistics.totalTime          )
{
}

GpUnitTestHandlerStatistics::GpUnitTestHandlerStatistics (GpUnitTestHandlerStatistics&& aStatistics) noexcept:
suiteStartFailedCount(std::move(aStatistics.suiteStartFailedCount)),
suiteStopFailedCount (std::move(aStatistics.suiteStopFailedCount )),
passedCount          (std::move(aStatistics.passedCount          )),
failedCount          (std::move(aStatistics.failedCount          )),
skippedCount         (std::move(aStatistics.skippedCount         )),
disabledCount        (std::move(aStatistics.disabledCount        )),
startTs              (std::move(aStatistics.startTs              )),
finishTs             (std::move(aStatistics.finishTs             )),
totalTime            (std::move(aStatistics.totalTime            ))
{
}

std::string GpUnitTestHandlerStatistics::SToString (const GpUnitTestHandlerStatistics& aStatistics)
{
    const GpUnitTestHandlerStatistics localCopy(aStatistics);

    std::string fmtOutBuffer;
    fmtOutBuffer.reserve(512);

    if (localCopy.suiteStartFailedCount > 0)
    {
        fmt::format_to
        (
            std::back_inserter(fmtOutBuffer),
            "[!!!!!!!!]: ENV START FAILED: {} <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n",
            std::to_string(localCopy.suiteStartFailedCount)
        );
    }

    if (localCopy.suiteStopFailedCount > 0)
    {
        fmt::format_to
        (
            std::back_inserter(fmtOutBuffer),
            "[!!!!!!!!]: ENV STOP FAILED: {} <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n",
            std::to_string(localCopy.suiteStopFailedCount)
        );
    }

    fmt::format_to
    (
        std::back_inserter(fmtOutBuffer),
        "[========]: PASSED:           {}\n" \
        "[========]: SKIPPED:          {}\n" \
        "[========]: DISABLED:         {}\n",
        std::to_string(localCopy.passedCount),
        std::to_string(localCopy.skippedCount),
        std::to_string(localCopy.disabledCount)
    );

    if (localCopy.failedCount == 0)
    {
        fmt::format_to
        (
            std::back_inserter(fmtOutBuffer),
            "[========]: FAILED:           {}\n",
            std::to_string(localCopy.failedCount)
        );
    } else
    {
        fmt::format_to
        (
            std::back_inserter(fmtOutBuffer),
            "[!!!!!!!!]: FAILED:           {} <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n",
            std::to_string(localCopy.failedCount)
        );
    }

    fmt::format_to
    (
        std::back_inserter(fmtOutBuffer),
        "[========]: Started at:       {}\n" \
        "[========]: Done at:          {}\n" \
        "[========]: Total time:       {:.6f}s.",
        GpDateTimeOps::SUnixTsToStr(localCopy.startTs, GpDateTimeFormat::STD_DATE_TIME),
        GpDateTimeOps::SUnixTsToStr(localCopy.finishTs, GpDateTimeFormat::STD_DATE_TIME),
        seconds_t(localCopy.totalTime).Value()
    );

    return std::string(std::data(fmtOutBuffer), std::size(fmtOutBuffer));
}

void    GpUnitTestHandlerStatistics::SSetSumm
(
    GpUnitTestHandlerStatistics&        aStatisticsAOut,
    const GpUnitTestHandlerStatistics&  aStatisticsB
)
{
    GpUniqueLock<GpMutex> uniqueLockA(aStatisticsAOut.iMutex);
    GpUniqueLock<GpMutex> uniqueLockB(aStatisticsB.iMutex);

    aStatisticsAOut.suiteStartFailedCount   = aStatisticsAOut.suiteStartFailedCount + aStatisticsB.suiteStartFailedCount;
    aStatisticsAOut.suiteStopFailedCount    = aStatisticsAOut.suiteStopFailedCount  + aStatisticsB.suiteStopFailedCount;
    aStatisticsAOut.passedCount             = aStatisticsAOut.passedCount           + aStatisticsB.passedCount;
    aStatisticsAOut.failedCount             = aStatisticsAOut.failedCount           + aStatisticsB.failedCount;
    aStatisticsAOut.skippedCount            = aStatisticsAOut.skippedCount          + aStatisticsB.skippedCount;
    aStatisticsAOut.disabledCount           = aStatisticsAOut.disabledCount         + aStatisticsB.disabledCount;
    aStatisticsAOut.startTs                 = unix_ts_ms_t::SMake(std::min(aStatisticsAOut.startTs.Value(), aStatisticsB.startTs.Value()));
    aStatisticsAOut.finishTs                = unix_ts_ms_t::SMake(std::max(aStatisticsAOut.finishTs.Value(), aStatisticsB.finishTs.Value()));
    aStatisticsAOut.totalTime               = aStatisticsAOut.totalTime             + aStatisticsB.totalTime;
}

GpUnitTestHandlerStatistics GpUnitTestHandlerStatistics::SSumm
(
    const GpUnitTestHandlerStatistics& aStatisticsA,
    const GpUnitTestHandlerStatistics& aStatisticsB
)
{
    GpUnitTestHandlerStatistics res;

    {
        GpUniqueLock<GpMutex> uniqueLockA(aStatisticsA.iMutex);
        GpUniqueLock<GpMutex> uniqueLockB(aStatisticsB.iMutex);

        res.suiteStartFailedCount   = aStatisticsA.suiteStartFailedCount+ aStatisticsB.suiteStartFailedCount;
        res.suiteStopFailedCount    = aStatisticsA.suiteStopFailedCount + aStatisticsB.suiteStopFailedCount;
        res.passedCount             = aStatisticsA.passedCount          + aStatisticsB.passedCount;
        res.failedCount             = aStatisticsA.failedCount          + aStatisticsB.failedCount;
        res.skippedCount            = aStatisticsA.skippedCount         + aStatisticsB.skippedCount;
        res.disabledCount           = aStatisticsA.disabledCount        + aStatisticsB.disabledCount;
        res.startTs                 = unix_ts_ms_t::SMake(std::min(aStatisticsA.startTs.Value(), aStatisticsB.startTs.Value()));
        res.finishTs                = unix_ts_ms_t::SMake(std::max(aStatisticsA.finishTs.Value(), aStatisticsB.finishTs.Value()));
        res.totalTime               = aStatisticsA.totalTime            + aStatisticsB.totalTime;
    }

    return res;
}

GpUnitTestHandlerStatistics&    GpUnitTestHandlerStatistics::operator= (const GpUnitTestHandlerStatistics& aStatistics) noexcept
{
    GpUniqueLock<GpMutex> uniqueLockThis(iMutex);
    GpUniqueLock<GpMutex> uniqueLockOther(aStatistics.iMutex);

    suiteStartFailedCount   = aStatistics.suiteStartFailedCount;
    suiteStopFailedCount    = aStatistics.suiteStopFailedCount;
    passedCount             = aStatistics.passedCount;
    failedCount             = aStatistics.failedCount;
    skippedCount            = aStatistics.skippedCount;
    disabledCount           = aStatistics.disabledCount;
    startTs                 = aStatistics.startTs;
    finishTs                = aStatistics.finishTs;
    totalTime               = aStatistics.totalTime;

    return *this;
}

GpUnitTestHandlerStatistics&    GpUnitTestHandlerStatistics::operator= (GpUnitTestHandlerStatistics&& aStatistics) noexcept
{
    GpUniqueLock<GpMutex> uniqueLockThis(iMutex);
    GpUniqueLock<GpMutex> uniqueLockOther(aStatistics.iMutex);

    suiteStartFailedCount   = std::move(aStatistics.suiteStartFailedCount);
    suiteStopFailedCount    = std::move(aStatistics.suiteStopFailedCount);
    passedCount             = std::move(aStatistics.passedCount);
    failedCount             = std::move(aStatistics.failedCount);
    skippedCount            = std::move(aStatistics.skippedCount);
    disabledCount           = std::move(aStatistics.disabledCount);
    startTs                 = std::move(aStatistics.startTs);
    finishTs                = std::move(aStatistics.finishTs);
    totalTime               = std::move(aStatistics.totalTime);

    return *this;
}

}// namespace GPlatform::UnitTest
