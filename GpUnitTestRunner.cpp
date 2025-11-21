#include <GpUnitTests/GpUnitTestRunner.hpp>
#include <GpUnitTests/GpUnitTestGroup.hpp>
#include <GpLog/GpLogCore/GpLog.hpp>
#include <GpUnitTests/Handlers/GpUnitTestLogOutHandlerFactory.hpp>

namespace GPlatform::UnitTest {

GpUnitTestRunner::GpUnitTestRunner
(
    const size_t    aId,
    ExecQueueT::SP  aExecQueueSP
):
GpTaskFiber{"Unit test runner["_sv + aId + "]"_sv},
iExecQueueSP{std::move(aExecQueueSP)}
{
}

GpUnitTestRunner::~GpUnitTestRunner (void)
{
}

GpUnitTestRunner::SP    GpUnitTestRunner::SRunnerByCurrentTask (void)
{
    GpTask::WP taskWP = GpTask::SCurrentTask();
    GpTask::SP taskSP = taskWP.Lock();

    VERIFY
    (
        taskSP.IsNotNULL(),
        "currentRunner is null"_sv
    );

    return taskSP.CastTo<GpUnitTestRunner::SP>();
}

void    GpUnitTestRunner::OnTestFailedExpect
(
    std::string_view        aMsg,
    const SourceLocationT&  aLocation
)
{
    iCurrentUnitTestGroupSP.V().OnTestFailedExpect(aMsg, aLocation);
}

void    GpUnitTestRunner::OnStart (void)
{
    iStatistics.startTs = GpDateTimeOps::SUnixTS_ms();

    // TODO: move to test setup or config
    iHandlerUP = GpUnitTestLogOutHandlerFactory{}.NewInstance();

    LOG_INFO(TaskName() + ": start..."_sv);
}

GpTaskRunRes::EnumT GpUnitTestRunner::OnStep (void)
{
    ExecQueueT& queue = iExecQueueSP.V();

    while(!IsStopRequested())
    {
        auto                            popRes          = queue.PopWaitFor(100.0_si_ms);
        GpUnitTestGroup::C::Opts::SP    testGroupOpt    = std::move(popRes.iValue);

        if (testGroupOpt.has_value() == false)
        {
            if (popRes.iFlags & (ExecQueueT::FlagsT(ExecQueueT::FlagE::INTERRUPT) | ExecQueueT::FlagsT(ExecQueueT::FlagE::STOP_PRODUCE)))
            {
                break;
            }

            continue;
        }

        // Extract result
        iCurrentUnitTestGroupSP = std::move(testGroupOpt.value());

        const GpUnitTestHandlerStatistics runStat = iCurrentUnitTestGroupSP.V().Run(*iHandlerUP);
        GpUnitTestHandlerStatistics::SSetSumm(iStatistics, runStat);
    }

    return GpTaskRunRes::DONE;
}

void    GpUnitTestRunner::OnStop (ExceptionsT& aStopExceptionsOut) noexcept
{
    try
    {
        LOG_INFO
        (
            fmt::format
            (
                "[GpUnitTestRunner::OnStop]: Name: '{}'",
                TaskName()
            )
        );

        DonePromise(GpMethodAccess{this}).Fulfill(GpAny{iStatistics});
    } catch (const GpException& ex)
    {
        aStopExceptionsOut.emplace_back(ex);
    } catch (const std::exception& ex)
    {
        aStopExceptionsOut.emplace_back(GpException{ex.what()});
    } catch (...)
    {
        aStopExceptionsOut.emplace_back(GpException{"[GpUnitTestRunner::OnStop]: unknown exception"_sv});
    }
}

void    GpUnitTestRunner::OnStopException (const GpException &aException) noexcept
{
    LOG_EXCEPTION
    (
        "[GpUnitTestRunner::OnStopException]",
        aException
    );
}

}// namespace GPlatform::UnitTest
