#pragma once

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>

namespace GPlatform::UnitTest {

class GpUnitTestSuiteGroup;

class GpUnitTest
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpUnitTest)
    CLASS_DD(GpUnitTest)

public:
    using FnT = std::function<void(GpUnitTestSuiteGroup& aUnitTestSuiteGroup)>;

public:
    inline              GpUnitTest  (FnT            aFn,
                                     std::string    aName,
                                     std::string    aComment) noexcept;
                        ~GpUnitTest (void) noexcept = default;

    std::string_view    Name        (void) const noexcept {return iName;}
    inline void         Run         (GpUnitTestSuiteGroup& aUnitTestSuiteGroup);

private:
    FnT                 iFn;
    std::string         iName;
    std::string         iComment;
};

GpUnitTest::GpUnitTest
(
    FnT         aFn,
    std::string aName,
    std::string aComment
) noexcept:
iFn     (std::move(aFn)),
iName   (std::move(aName)),
iComment(std::move(aComment))
{
}

void    GpUnitTest::Run (GpUnitTestSuiteGroup& aUnitTestSuiteGroup)
{
    if (iFn) [[likely]]
    {
        iFn(aUnitTestSuiteGroup);
    }
}

}// namespace GPlatform::UnitTest
