#pragma once

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>

#include <boost/type_index.hpp>

namespace GPlatform::UnitTest {

class GpUnitTestHandler;
class GpUnitTestGroup;

class GpUnitTestSuiteGroup
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpUnitTestSuiteGroup)
    CLASS_DD(GpUnitTestSuiteGroup)

public:
    using OnStartHookFnT        = std::function<void(GpUnitTestHandler& aHandler,
                                                     GpUnitTestGroup&   aGroup)>;

    template<typename T>
    class TypeToName
    {
    public:
        static std::string SName()
        {
            return std::string(boost::typeindex::type_id<T>().pretty_name());
        }
    };

protected:
    template<typename InheritedT>
                                GpUnitTestSuiteGroup    (TypeToName<InheritedT> aTypeToName): GpUnitTestSuiteGroup(decltype(aTypeToName)::SName()){}
    inline                      GpUnitTestSuiteGroup    (std::string    aName) noexcept;

public:
    virtual                     ~GpUnitTestSuiteGroup   (void) noexcept = default;

    std::string_view            Name                    (void) const noexcept {return iName;}
    void                        AddStartHook            (OnStartHookFnT&& aFn) {iOnStartHooks.emplace_back(std::move(aFn));}

    inline void                 BeforeTests             (GpUnitTestHandler& aHandler,
                                                         GpUnitTestGroup&   aGroup);
    inline void                 AfterTests              (void);

protected:
    virtual void                _BeforeTests            (void) {};
    virtual void                _AfterTests             (void) {};

private:
    const std::string           iName;
    std::vector<OnStartHookFnT> iOnStartHooks;
};

GpUnitTestSuiteGroup::GpUnitTestSuiteGroup (std::string aName) noexcept:
iName(std::move(aName))
{
}

void    GpUnitTestSuiteGroup::BeforeTests
(
    GpUnitTestHandler&  aHandler,
    GpUnitTestGroup&    aGroup
)
{
    _BeforeTests();

    //Call "before" hooks
    for (OnStartHookFnT& fn: iOnStartHooks)
    {
        if (fn)
        {
            fn(aHandler, aGroup);
        }
    }
}

void    GpUnitTestSuiteGroup::AfterTests (void)
{
    _AfterTests();
}

}// namespace GPlatform::UnitTest
