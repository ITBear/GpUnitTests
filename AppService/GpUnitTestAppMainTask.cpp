#include <GpUnitTests/AppService/GpUnitTestAppMainTask.hpp>
#include <GpUnitTests/AppService/GpUnitTestAppCfgDesc.hpp>
#include <GpUnitTests/AppService/GpUnitTestAppCmdArgsDesc.hpp>
#include <GpUnitTests/GpUnitTestManager.hpp>
#include <GpLog/GpLogCore/GpLog.hpp>

namespace GPlatform::UnitTest {

GpUnitTestAppMainTask::GpUnitTestAppMainTask (void) noexcept
{
}

GpUnitTestAppMainTask::~GpUnitTestAppMainTask (void) noexcept
{
}

void    GpUnitTestAppMainTask::OnStart (void)
{
    LOG_INFO("[GpUnitTestAppMainTask::OnStart]: start..."_sv);

    LOG_INFO("[GpUnitTestAppMainTask::OnStart]: successfully started"_sv);
}

GpTaskRunRes::EnumT GpUnitTestAppMainTask::OnStep (void)
{
    LOG_INFO("[GpUnitTestAppMainTask::OnStep]:..."_sv);

    // Run tests
    GpUnitTestManager::S().RunAndWaitForDone();

    return GpTaskRunRes::DONE;
}

void    GpUnitTestAppMainTask::OnStop (ExceptionsT& aStopExceptionsOut) noexcept
{
    try
    {
        LOG_INFO
        (
            fmt::format
            (
                "[GpUnitTestAppMainTask::OnStop]: {}",
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
        aStopExceptionsOut.emplace_back(GpException{"[GpUnitTestAppMainTask::OnStop]: unknown exception"_sv});
    }

    GpServiceMainTask::OnStop(aStopExceptionsOut);
}

void    GpUnitTestAppMainTask::OnStopException (const GpException& aException) noexcept
{
    LOG_EXCEPTION
    (
        "[GpUnitTestAppMainTask::OnStopException]",
        aException
    );
}

}// namespace GPlatform::UnitTest
