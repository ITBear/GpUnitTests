#pragma once

#include <GpUnitTests/GpUnitTestGroup.hpp>
#include <GpUnitTests/GpUnitTestSuiteGroup.hpp>
#include <GpUnitTests/Handlers/GpUnitTestHandlerFactory.hpp>
#include <GpCore2/GpTasks/ITC/GpItcFuture.hpp>
#include <GpCore2/GpTasks/ITC/GpItcQueueMPMC.hpp>
#include <GpCore2/GpTasks/GpTask.hpp>

namespace GPlatform::UnitTest {

class GpUnitTestAppCmdArgsDesc;

class GP_UNIT_TESTS_API GpUnitTestManager
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpUnitTestManager)
    CLASS_DD(GpUnitTestManager)
    TAG_SET(THREAD_SAFE)

    using DoneFutureT                   = GpTask::DoneFutureT;
    using ExecQueueT                    = GpItcQueueMPMC<GpUnitTestGroup::SP>;
    using UnitTestSuiteGroupFactoryFnT  = std::function<GpUnitTestSuiteGroup::SP()>;

private:
                                GpUnitTestManager   (void) noexcept;

public:
                                ~GpUnitTestManager  (void) noexcept;

    static GpUnitTestManager&   S                   (void);

    void                        RunAndWaitForDone   (void) noexcept;

    template<typename UnitTestSuiteGroupT>
    void                        AddGroupTest        (GpUnitTest::FnT    aTestFn,
                                                     std::string        aGroupName,
                                                     std::string        aTestName,
                                                     std::string        aTestCommment);

private:
    void                        AddGroupTest        (UnitTestSuiteGroupFactoryFnT   aUnitTestSuiteGroupFactoryFn,
                                                     std::string_view               aUnitTestSuiteGroupTypeDemangleName,
                                                     GpUnitTest::FnT                aTestFn,
                                                     std::string                    aGroupName,
                                                     std::string                    aTestName,
                                                     std::string                    aTestCommment);
    DoneFutureT::C::Vec::SP     StartRunners        (ExecQueueT::SP aExecQueueSP) noexcept;
    void                        RunTests            (ExecQueueT::SP aExecQueueSP) noexcept;
    GpUnitTestHandlerStatistics::C::Vec::Val
                                WaitForRunners      (DoneFutureT::C::Vec::SP&   aTestRunnerDoneFutures);
    bool                        OnDone              (const GpUnitTestHandlerStatistics::C::Vec::Val&    aStatistics,
                                                     GpUnitTestHandler&                                 aManagerHandler);

private:
    mutable GpMutex<>               iMutex;
    GpUnitTestGroup::C::MapStr::SP  iTestGroups     GUARDED_BY(iMutex);
    bool                            iIsEnableToAdd  GUARDED_BY(iMutex) = true;
};

template<typename UnitTestSuiteGroupT>
void    GpUnitTestManager::AddGroupTest
(
    GpUnitTest::FnT aTestFn,
    std::string     aGroupName,
    std::string     aTestName,
    std::string     aTestCommment
)
{
    static const UnitTestSuiteGroupFactoryFnT sFactoryFn = []() -> GpUnitTestSuiteGroup::SP
    {
        return MakeSP<UnitTestSuiteGroupT>();
    };

    AddGroupTest
    (
        sFactoryFn,
        typeid(UnitTestSuiteGroupT).name(),
        std::move(aTestFn),
        std::move(aGroupName),
        std::move(aTestName),
        std::move(aTestCommment)
    );
}

template<typename UnitTestClassT>
class GpUnitTest_Registrator
{
public:
    GpUnitTest_Registrator
    (
        GpUnitTest::FnT aTestFn,
        std::string     aGroupName,
        std::string     aTestName,
        std::string     aTestCommment
    )
    {
        GpUnitTestManager::S().AddGroupTest<UnitTestClassT>
        (
            std::move(aTestFn),
            std::move(aGroupName),
            std::move(aTestName),
            std::move(aTestCommment)
        );
    }
};

}// namespace GPlatform::UnitTest

#define GP_TEST(UnitTestClass, TestFn, TestComment) \
\
    static void UnitTestClass##_##TestFn([[maybe_unused]] UnitTestClass& _ctx); \
    static void UnitTestClass##_##TestFn##CCtx(GpUnitTestSuiteGroup& aUnitTestSuiteGroup) \
    { \
        UnitTestClass##_##TestFn(static_cast<UnitTestClass&>(aUnitTestSuiteGroup)); \
    } \
\
    static ::GPlatform::UnitTest::GpUnitTest_Registrator<UnitTestClass> UnitTestClass##_##TestFn##_Registrator \
    ( \
        UnitTestClass##_##TestFn##CCtx, \
        #UnitTestClass, \
        #TestFn, \
        TestComment \
    ); \
\
    static void UnitTestClass##_##TestFn([[maybe_unused]] UnitTestClass& _ctx)
