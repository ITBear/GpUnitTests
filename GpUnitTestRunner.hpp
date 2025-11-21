#pragma once

#include <GpUnitTests/Handlers/GpUnitTestHandlerFactory.hpp>
#include <GpUnitTests/GpUnitTestGroup.hpp>
#include <GpCore2/GpTasks/ITC/GpItcQueueMPMC.hpp>
#include <GpCore2/GpTasks/Fibers/GpTaskFiber.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpSharedMap.hpp>

namespace GPlatform::UnitTest {

class GP_UNIT_TESTS_API GpUnitTestRunner final: public GpTaskFiber
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpUnitTestRunner)
    CLASS_DD(GpUnitTestRunner)

    using ExecQueueT = GpItcQueueMPMC<GpUnitTestGroup::SP>;

public:
                                GpUnitTestRunner        (size_t         aId,
                                                         ExecQueueT::SP aExecQueueSP);
    virtual                     ~GpUnitTestRunner       (void) override final;

    static GpUnitTestRunner::SP SRunnerByCurrentTask    (void);
    void                        OnTestFailedExpect      (std::string_view       aMsg,
                                                         const SourceLocationT& aLocation);

    virtual void                OnStart                 (void) override final;
    virtual GpTaskRunRes::EnumT OnStep                  (void) override final;
    virtual void                OnStop                  (ExceptionsT& aStopExceptionsOut) noexcept override final;
    virtual void                OnStopException         (const GpException &aException) noexcept override final;

private:
    ExecQueueT::SP              iExecQueueSP;
    GpUnitTestHandler::UP       iHandlerUP;
    GpUnitTestGroup::SP         iCurrentUnitTestGroupSP;
    GpUnitTestHandlerStatistics iStatistics;
};

}// namespace GPlatform::UnitTest
