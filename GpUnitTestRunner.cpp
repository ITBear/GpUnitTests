#include <GpUnitTests/GpUnitTestRunner.hpp>
#include <GpUnitTests/GpUnitTestGroup.hpp>
#include <GpLog/GpLogCore/GpLog.hpp>

namespace GPlatform::UnitTest {

std::vector<GpUnitTestRunner*>  GpUnitTestRunner::sRunners;

GpUnitTestRunner::GpUnitTestRunner
(
    const size_t                    aId,
    std::atomic_bool&               aIsProduceDone,
    SharedQueueT::SP                aConsumerQueue,
    GpUnitTestHandlerFactory::SP    aHandlerFactory,
    GpUnitTestHandlerStatistics&    aStatisticsOut
):
GpTaskFiber{"Unit test runner["_sv + aId + "]"_sv},
iIsProduceDoneRef{aIsProduceDone},
iConsumerQueue   {std::move(aConsumerQueue)},
iHandlerFactory  {std::move(aHandlerFactory)},
iStatisticsOut   {aStatisticsOut}
{
}

GpUnitTestRunner::~GpUnitTestRunner (void)
{
}

GpUnitTestRunner&   GpUnitTestRunner::SRunnerByCurrentTask (void)
{
    GpTask& currentTask = GpTask::SCurrentTask().value().get();
    auto runnerOpt = currentTask.GetVarRef("_utr_");

    THROW_COND_GP
    (
        runnerOpt.has_value(),
        "currentRunner is null"_sv
    );

    GpUnitTestRunner* runnerPtr = runnerOpt.value().get().Value<GpUnitTestRunner*>();

    THROW_COND_GP
    (
        runnerPtr != nullptr,
        "currentRunner is null"_sv
    );

    return *runnerPtr;
}

void    GpUnitTestRunner::OnTestFailedExpect
(
    std::string_view        aMsg,
    const SourceLocationT&  aLocation
)
{
    iCurrentUnitTestGroup.V().OnTestFailedExpect(aMsg, aLocation);
}

void    GpUnitTestRunner::OnStart (void)
{
    // Save current task to _utr_ var
    GpTask& currentTask = GpTask::SCurrentTask().value().get();
    currentTask.SetVar("_utr_", this);

    //
    iStatisticsOut.startTs = GpDateTimeOps::SUnixTS_ms();

    //
    LOG_INFO(TaskName() + ": START..."_sv);
}

GpTaskRunRes::EnumT GpUnitTestRunner::OnStep (void)
{
    // Consume next GpUnitTestGroup
    GpUnitTestGroup::C::Opt::SP testGroupOpt = iConsumerQueue->WaitAndPop(0.5_si_s);

    if (!testGroupOpt.has_value())
    {
        if (iIsProduceDoneRef)
        {
            return GpTaskRunRes::DONE;
        } else
        {
            return GpTaskRunRes::READY_TO_RUN;
        }
    }

    // Extract result
    GpUnitTestGroup::SP unitTestGroupSP = testGroupOpt.value();

    // Do
    if (unitTestGroupSP.IsNotNULL())
    {
        iCurrentUnitTestGroup = unitTestGroupSP;

        GpUnitTestHandler::SP               handler = iHandlerFactory.V().NewInstance();
        const GpUnitTestHandlerStatistics   runStat = iCurrentUnitTestGroup.V().Run(handler.V());

        GpUnitTestHandlerStatistics::SSetSumm(iStatisticsOut, runStat);
        iCurrentUnitTestGroup.Clear();
    }

    return GpTaskRunRes::READY_TO_RUN;
}

void    GpUnitTestRunner::OnStop (StopExceptionsT& aStopExceptionsOut) noexcept
{
    try
    {
        LOG_INFO
        (
            fmt::format
            (
                "[GpUnitTestRunner::OnStop]: {}",
                TaskName()
            )
        );
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
