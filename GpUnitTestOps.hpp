#pragma once

#include <GpCore2/Config/IncludeExt/fmt.hpp>

#include "GpUnitTestAssert.hpp"
#include "GpUnitTestRunner.hpp"

#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

namespace GPlatform::UnitTest {

enum class GpUnitTestFailMode
{
    ASSERT,
    EXPECT
};

template<typename           T1,
         typename           T2,
         GpUnitTestFailMode Mode>
void    _ON_TEST_FAILED
(
    const T1&               aArg1,
    const T2&               aArg2,
    std::string_view        aCompareName,
    std::string_view        aArg1AsSrcText,
    std::string_view        aArg2AsSrcText,
    std::string_view        aUserMessage,
    const SourceLocationT&  aLocation
)
{
    std::string modeStr;
    if constexpr (Mode == GpUnitTestFailMode::ASSERT)
    {
        modeStr = "must be";
    } else
    {
        modeStr = "expected to be";
    }

    std::string message = fmt::format
    (
        //'aArg1AsSrcText'='Arg1' expected to be EQUAL to 'Arg2'. {}
        "'{}'='{}' {} {} to '{}'='{}'. {}",
        aArg1AsSrcText,
        std::to_string(aArg1),
        modeStr,
        aCompareName,
        aArg2AsSrcText,
        std::to_string(aArg2),
        aUserMessage
    );

    if constexpr (Mode == GpUnitTestFailMode::ASSERT)
    {
        throw GpUnitTestAssert(message, aLocation);
    } else
    {
        GpUnitTestRunner::SRunnerByCurrentTask().OnTestFailedExpect(message, aLocation);
    }
}

template<typename           T1,
         typename           T2,
         GpUnitTestFailMode Mode>
void    _CMP_VALUES
(
    const T1&                                   aArg1,
    const T2&                                   aArg2,
    std::string_view                            aUserMessageOnError,
    const SourceLocationT&                      aLocation,
    std::function<bool(const T1&, const T2&)>   aCmpFn,
    std::string_view                            aCmpName,
    std::string_view                            aArg1AsSrcText,
    std::string_view                            aArg2AsSrcText
)
{
    if (aCmpFn(aArg1, aArg2)) [[likely]]
    {
        return;
    }

    _ON_TEST_FAILED<T1, T2, Mode>
    (
        aArg1,
        aArg2,
        aCmpName,
        aArg1AsSrcText,
        aArg2AsSrcText,
        aUserMessageOnError,
        aLocation
    );
}

// ----------------------- TRUE -----------------------
template<GpUnitTestFailMode Mode>
void    _CHECK_IF_TRUE
(
    const bool              aArg1,
    std::string_view        aUserMessageOnError,
    std::string_view        aArg1AsSrcText,
    const SourceLocationT&  aLocation
)
{
    const bool _t = true;
    _CMP_VALUES<bool, bool, Mode>
    (
        aArg1,
        _t,
        aUserMessageOnError,
        aLocation,
        [](const auto& a, const auto& b)->bool{return a == b;},
        "EQUAL"_sv,
        aArg1AsSrcText,
        "true"_sv
    );
}

inline void _EXPECT_TRUE
(
    const bool              aArg1,
    std::string_view        aUserMessageOnError,
    std::string_view        aArg1AsSrcText,
    const SourceLocationT&  aLocation           = SourceLocationT::current()
)
{
    _CHECK_IF_TRUE<GpUnitTestFailMode::EXPECT>
    (
        aArg1,
        aUserMessageOnError,
        aArg1AsSrcText,
        aLocation
    );
}

#define EXPECT_TRUE(ARG1)                   _EXPECT_TRUE(ARG1, {},           std::string_view(#ARG1))
#define EXPECT_TRUE_MSG(ARG1, USER_MESSAGE) _EXPECT_TRUE(ARG1, USER_MESSAGE, std::string_view(#ARG1))

inline void _ASSERT_TRUE
(
    const bool              aArg1,
    std::string_view        aUserMessageOnError,
    std::string_view        aArg1AsSrcText,
    const SourceLocationT&  aLocation = SourceLocationT::current()
)
{
    _CHECK_IF_TRUE<GpUnitTestFailMode::ASSERT>
    (
        aArg1,
        aUserMessageOnError,
        aArg1AsSrcText,
        aLocation
    );
}

#define ASSERT_TRUE(ARG1)                   _ASSERT_TRUE(ARG1, {},           std::string_view(#ARG1))
#define ASSERT_TRUE_MSG(ARG1, USER_MESSAGE) _ASSERT_TRUE(ARG1, USER_MESSAGE, std::string_view(#ARG1))

// ----------------------- FALSE -----------------------
template<GpUnitTestFailMode Mode>
void    _CHECK_IF_FALSE
(
    const bool              aArg1,
    std::string_view        aUserMessageOnError,
    std::string_view        aArg1AsSrcText,
    const SourceLocationT&  aLocation
)
{
    const bool _f = false;
    _CMP_VALUES<bool, bool, Mode>
    (
        aArg1,
        _f,
        aUserMessageOnError,
        aLocation,
        [](const auto& a, const auto& b)->bool{return a == b;},
        "EQUAL"_sv,
        aArg1AsSrcText,
        "false"_sv
    );
}

inline void _EXPECT_FALSE
(
    const bool              aArg1,
    std::string_view        aUserMessageOnError,
    std::string_view        aArg1AsSrcText,
    const SourceLocationT&  aLocation = SourceLocationT::current()
)
{
    _CHECK_IF_FALSE<GpUnitTestFailMode::EXPECT>
    (
        aArg1,
        aUserMessageOnError,
        aArg1AsSrcText,
        aLocation
    );
}

#define EXPECT_FALSE(ARG1)                   _EXPECT_FALSE(ARG1, {},           std::string_view(#ARG1))
#define EXPECT_FALSE_MSG(ARG1, USER_MESSAGE) _EXPECT_FALSE(ARG1, USER_MESSAGE, std::string_view(#ARG1))

inline void _ASSERT_FALSE
(
    const bool              aArg1,
    std::string_view        aUserMessageOnError,
    std::string_view        aArg1AsSrcText,
    const SourceLocationT&  aLocation = SourceLocationT::current()
)
{
    _CHECK_IF_FALSE<GpUnitTestFailMode::ASSERT>
    (
        aArg1,
        aUserMessageOnError,
        aArg1AsSrcText,
        aLocation
    );
}

#define ASSERT_FALSE(ARG1)                   _ASSERT_FALSE(ARG1, {},           std::string_view(#ARG1))
#define ASSERT_FALSE_MSG(ARG1, USER_MESSAGE) _ASSERT_FALSE(ARG1, USER_MESSAGE, std::string_view(#ARG1))

// ------------------- EXCEPTION --------------------
template<typename T>
void    _EXPECT_EXCEPTION
(
    std::function<void()>   aFn,
    std::string_view        aUserMessageOnError,
    std::string_view        aFnAsSrcText,
    const SourceLocationT&  aLocation = SourceLocationT::current()
)
{
    bool isCatched = false;

    try
    {
        aFn();
    } catch(const T&)
    {
        isCatched = true;
    }

    _EXPECT_TRUE
    (
        isCatched,
        aUserMessageOnError,
        aFnAsSrcText,
        aLocation
    );
}

#define EXPECT_EXCEPTION(T, FN_TO_CALL, USER_MESSAGE) _EXPECT_EXCEPTION<T>(FN_TO_CALL, USER_MESSAGE, std::string_view(#FN_TO_CALL))

template<typename T>
void    _EXPECT_EXCEPTION
(
    std::function<void()>                               aFn,
    std::function<std::optional<std::string>(const T&)> aCheckExFn,
    std::string_view                                    aUserMessageOnError,
    std::string_view                                    aFnAsSrcText,
    const SourceLocationT&                              aLocation = SourceLocationT::current()
)
{
    bool isCatched = false;

    try
    {
        aFn();
    } catch(const T& e)
    {
        std::optional<std::string> checkExRes = aCheckExFn(e);

        if (checkExRes.has_value())
        {
            _EXPECT_TRUE
            (
                false,
                "Exception expected: "_sv + checkExRes.value(),
                aFnAsSrcText,
                aLocation
            );

            return;
        }

        isCatched = true;
    }

    _EXPECT_TRUE
    (
        isCatched,
        aUserMessageOnError,
        aFnAsSrcText,
        aLocation
    );
}

#define EXPECT_EXCEPTION_CHECK(T, FN_TO_CALL, FN_TO_CHECK, USER_MESSAGE) _EXPECT_EXCEPTION<T>(FN_TO_CALL, FN_TO_CHECK, USER_MESSAGE, std::string_view(#FN_TO_CALL))

template<typename T>
void    _ASSERT_EXCEPTION
(
    std::function<void()>   aFn,
    std::string_view        aUserMessageOnError,
    std::string_view        aFnAsSrcText,
    const SourceLocationT&  aLocation = SourceLocationT::current()
)
{
    bool isCatched = false;

    try
    {
        aFn();
    } catch(const T&)
    {
        isCatched = true;
    }

    _ASSERT_TRUE
    (
        isCatched,
        aUserMessageOnError,
        aFnAsSrcText,
        aLocation
    );
}

#define ASSERT_EXCEPTION(T, FN_TO_CALL, USER_MESSAGE) _ASSERT_EXCEPTION<T>(FN_TO_CALL, USER_MESSAGE, std::string_view(#FN_TO_CALL))

template<typename T>
void    _ASSERT_EXCEPTION
(
    std::function<void()>                               aFn,
    std::function<std::optional<std::string>(const T&)> aCheckExFn,
    std::string_view                                    aUserMessageOnError,
    std::string_view                                    aFnAsSrcText,
    const SourceLocationT&                              aLocation = SourceLocationT::current()
)
{
    bool isCatched = false;

    try
    {
        aFn();
    } catch(const T& e)
    {
        std::optional<std::string> checkExRes = aCheckExFn(e);

        if (checkExRes.has_value())
        {
            _ASSERT_TRUE
            (
                false,
                "Exception expected: "_sv + checkExRes.value(),
                aFnAsSrcText,
                aLocation
            );

            return;
        }

        isCatched = true;
    }

    _ASSERT_TRUE
    (
        isCatched,
        "Exception expected: "_sv + aUserMessageOnError,
        aFnAsSrcText,
        aLocation
    );
}

#define ASSERT_EXCEPTION_CHECK(T, FN_TO_CALL, FN_TO_CHECK, USER_MESSAGE) _ASSERT_EXCEPTION<T>(FN_TO_CALL, FN_TO_CHECK, USER_MESSAGE, std::string_view(#FN_TO_CALL))

// ----------------------- EQ -----------------------
template<typename           T1,
         typename           T2,
         GpUnitTestFailMode Mode>
void    _CHECK_EQ
(
    const T1&               aArg1,
    const T2&               aArg2,  
    std::string_view        aUserMessageOnError,
    std::string_view        aArg1AsSrcText,
    std::string_view        aArg2AsSrcText,
    const SourceLocationT&  aLocation
)
{
    _CMP_VALUES<T1, T2, Mode>
    (
        aArg1,
        aArg2,
        aUserMessageOnError,
        aLocation,
        [](const auto& a, const auto& b)->bool{return a == b;},
        "EQUAL"_sv,
        aArg1AsSrcText,
        aArg2AsSrcText
    );
}

template<typename T1,
         typename T2>
void    _EXPECT_EQ
(
    const T1&               aArg1,
    const T2&               aArg2,
    std::string_view        aUserMessageOnError,
    std::string_view        aArg1AsSrcText,
    std::string_view        aArg2AsSrcText,
    const SourceLocationT&  aLocation = SourceLocationT::current()
)
{
    _CHECK_EQ<T1, T2, GpUnitTestFailMode::EXPECT>
    (
        aArg1,
        aArg2,
        aUserMessageOnError,
        aArg1AsSrcText,
        aArg2AsSrcText,
        aLocation
    );
}

#define EXPECT_EQ(ARG1, ARG2)                   _EXPECT_EQ(ARG1, ARG2, {},           std::string_view(#ARG1), std::string_view(#ARG2))
#define EXPECT_EQ_MSG(ARG1, ARG2, USER_MESSAGE) _EXPECT_EQ(ARG1, ARG2, USER_MESSAGE, std::string_view(#ARG1), std::string_view(#ARG2))

template<typename T1,
         typename T2>
void    _ASSERT_EQ
(
    const T1&               aArg1,
    const T2&               aArg2,  
    std::string_view        aUserMessageOnError,
    std::string_view        aArg1AsSrcText,
    std::string_view        aArg2AsSrcText,
    const SourceLocationT&  aLocation = SourceLocationT::current()
)
{
    _CHECK_EQ<T1, T2, GpUnitTestFailMode::ASSERT>
    (
        aArg1,
        aArg2,
        aUserMessageOnError,
        aArg1AsSrcText,
        aArg2AsSrcText,
        aLocation
    );
}

#define ASSERT_EQ(ARG1, ARG2)                   _ASSERT_EQ(ARG1, ARG2, {},           std::string_view(#ARG1), std::string_view(#ARG2))
#define ASSERT_EQ_MSG(ARG1, ARG2, USER_MESSAGE) _ASSERT_EQ(ARG1, ARG2, USER_MESSAGE, std::string_view(#ARG1), std::string_view(#ARG2))

// ----------------------- NOT EQ -----------------------
template<typename           T1,
         typename           T2,
         GpUnitTestFailMode Mode>
void    _CHECK_NOT_EQ
(
    const T1&               aArg1,
    const T2&               aArg2,
    std::string_view        aUserMessageOnError,
    std::string_view        aArg1AsSrcText,
    std::string_view        aArg2AsSrcText,
    const SourceLocationT&  aLocation
)
{
    _CMP_VALUES<T1, T2, Mode>
    (
        aArg1,
        aArg2,
        aUserMessageOnError,
        aLocation,
        [](const auto& a, const auto& b)->bool{return a != b;},
        "NOT EQUAL"_sv,
        aArg1AsSrcText,
        aArg2AsSrcText
    );
}

template<typename T1,
         typename T2>
void    _EXPECT_NOT_EQ
(
    const T1&               aArg1,
    const T2&               aArg2,
    std::string_view        aUserMessageOnError,
    std::string_view        aArg1AsSrcText,
    std::string_view        aArg2AsSrcText,
    const SourceLocationT&  aLocation = SourceLocationT::current()
)
{
    _CHECK_NOT_EQ<T1, T2, GpUnitTestFailMode::EXPECT>
    (
        aArg1,
        aArg2,
        aUserMessageOnError,
        aArg1AsSrcText,
        aArg2AsSrcText,
        aLocation
    );
}

#define EXPECT_NOT_EQ(ARG1, ARG2)                   _EXPECT_NOT_EQ(ARG1, ARG2, {},           std::string_view(#ARG1), std::string_view(#ARG2))
#define EXPECT_NOT_EQ_MSG(ARG1, ARG2, USER_MESSAGE) _EXPECT_NOT_EQ(ARG1, ARG2, USER_MESSAGE, std::string_view(#ARG1), std::string_view(#ARG2))

template<typename T1,
         typename T2>
void    _ASSERT_NOT_EQ
(
    const T1&               aArg1,
    const T2&               aArg2,
    std::string_view        aUserMessageOnError,
    std::string_view        aArg1AsSrcText,
    std::string_view        aArg2AsSrcText,
    const SourceLocationT&  aLocation = SourceLocationT::current()
)
{
    _CHECK_NOT_EQ<T1, T2, GpUnitTestFailMode::ASSERT>
    (
        aArg1,
        aArg2,
        aUserMessageOnError,
        aArg1AsSrcText,
        aArg2AsSrcText,
        aLocation
    );
}

#define ASSERT_NOT_EQ(ARG1, ARG2)                   _ASSERT_NOT_EQ(ARG1, ARG2, {},           std::string_view(#ARG1), std::string_view(#ARG2))
#define ASSERT_NOT_EQ_MSG(ARG1, ARG2, USER_MESSAGE) _ASSERT_NOT_EQ(ARG1, ARG2, USER_MESSAGE, std::string_view(#ARG1), std::string_view(#ARG2))

}// namespace GPlatform::UnitTest
