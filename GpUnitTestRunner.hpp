#pragma once

#include <GpUnitTests/Handlers/GpUnitTestHandlerFactory.hpp>
#include <GpUnitTests/GpUnitTestGroup.hpp>
#include <GpCore2/GpTasks/ITC/GpItcSharedQueue.hpp>
#include <GpCore2/GpTasks/Fibers/GpTaskFiber.hpp>

namespace GPlatform::UnitTest {

class GP_UNIT_TESTS_API GpUnitTestRunner final: public GpTaskFiber
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpUnitTestRunner)
    CLASS_DD(GpUnitTestRunner)

    using SharedQueueT = GpItcSharedQueue<GpUnitTestGroup::SP>;

public:
                                    GpUnitTestRunner        (size_t                         aId,
                                                             std::atomic_bool&              aIsProduceDone,
                                                             SharedQueueT::SP               aConsumerQueue,
                                                             GpUnitTestHandlerFactory::SP   aHandlerFactory,
                                                             GpUnitTestHandlerStatistics&   aStatisticsOut);
    virtual                         ~GpUnitTestRunner       (void) override final;

    static GpUnitTestRunner&        SRunnerByCurrentTask    (void);
    void                            OnTestFailedExpect      (std::string_view       aMsg,
                                                             const SourceLocationT& aLocation);

    virtual void                    OnStart                 (void) override final;
    virtual GpTaskRunRes::EnumT     OnStep                  (void) override final;
    virtual void                    OnStop                  (StopExceptionsT& aStopExceptionsOut) noexcept override final;
    virtual void                    OnStopException         (const GpException &aException) noexcept override final;

private:
    std::atomic_bool&               iIsProduceDoneRef;
    SharedQueueT::SP                iConsumerQueue;
    GpUnitTestHandlerFactory::SP    iHandlerFactory;
    GpUnitTestGroup::SP             iCurrentUnitTestGroup;
    GpUnitTestHandlerStatistics&    iStatisticsOut;

    static std::vector<GpUnitTestRunner*>   sRunners;
};

}// namespace GPlatform::UnitTest
