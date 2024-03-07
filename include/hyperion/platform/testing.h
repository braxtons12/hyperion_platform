/// @file testing.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief wrapper header for Doctest testing to ensure consistent configuration
/// @version 0.1
/// @date 2024-01-27
///
/// MIT License
/// @copyright Copyright (c) 2024 Braxton Salyer <braxtonsalyer@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to
/// deal in the Software without restriction, including without limitation the
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
/// IN THE SOFTWARE.
#ifndef HYPERION_PLATFORM_TESTING_H
#define HYPERION_PLATFORM_TESTING_H

#include <hyperion/platform/def.h>
#include <hyperion/platform/ignore.h>

#if HYPERION_ENABLE_TESTING

    #define DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES
    #define DOCTEST_CONFIG_NO_COMPARISON_WARNING_SUPPRESSION
    #define DOCTEST_CONFIG_USE_STD_HEADERS
    #define DOCTEST_CONFIG_SUPER_FAST_ASSERTS
    #include <doctest/doctest.h>

    /// @brief Use to define a doctest test suite.
    /// Usage is identical to `DOCTEST_TEST_SUITE()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define TEST_SUITE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START;               \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START;                   \
        DOCTEST_TEST_SUITE(__VA_ARGS__)                                   \
        /** NOLINT(modernize-use-trailing-return-type) **/                \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP                 \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP
    /// @brief Use to define a doctest test case.
    /// Usage is identical to `DOCTEST_TEST_CASE()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define TEST_CASE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START;              \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START;                  \
        /** NOLINTNEXTLINE **/                                           \
        DOCTEST_TEST_CASE(__VA_ARGS__)                                   \
        /** NOLINT **/                                                   \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP                \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP
    /// @brief Use to define a doctest test subcase.
    /// Usage is identical to `DOCTEST_SUBCASE()`, this just ignores particular warnings coming from
    /// doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define SUBCASE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START;            \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START;                \
        /** NOLINTNEXTLINE **/                                         \
        DOCTEST_SUBCASE(__VA_ARGS__)                                   \
        /** NOLINT **/                                                 \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP              \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP
#else

    #define IMPL_CONCAT(x, y)   x##y
    #define CONCAT2(x, y)       IMPL_CONCAT(x, y)
    #define CONCAT3(x, y, z)    CONCAT2(x, CONCAT2(y, z))
    #define CONCAT4(x, y, z, w) CONCAT2(x, CONCAT3(y, z, w))

    /// @brief Use to define a doctest test suite.
    /// Usage is identical to `DOCTEST_TEST_SUITE()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define TEST_SUITE(...)     /** NOLINT(cppcoreguidelines-macro-usage) **/     \
        constexpr auto CONCAT4(HYPERION_UNIQUE_IDENT, __LINE__, _, __COUNTER__) = \
            []()           /** NOLINT(modernize-use-trailing-return-type) **/
    /// @brief Use to define a doctest test case.
    /// Usage is identical to `DOCTEST_TEST_CASE()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define TEST_CASE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/        \
        /** NOLINTNEXTLINE **/                                                  \
        constexpr auto CONCAT4(HYPERION_UNIQUE_IDENT, __LINE__, _, __COUNTER__) \
            = [&]()      /** NOLINT **/
    /// @brief Use to define a doctest test subcase.
    /// Usage is identical to `DOCTEST_SUBCASE()`, this just ignores particular warnings coming from
    /// doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define SUBCASE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        /** NOLINTNEXTLINE **/                                         \
        TEST_CASE(__VA_ARGS__)                                         \
        /** NOLINT **/

    #undef CONCAT4
    #undef CONCAT3
    #undef CONCAT2
    #undef IMPL_CONCAT

#endif

#if HYPERION_ENABLE_TESTING && !defined(DOCTEST_CONFIG_DISABLE)
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_CHECK()`, this just ignores particular warnings coming from
    /// doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define CHECK(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START;          \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START;              \
        DOCTEST_CHECK(__VA_ARGS__)                                   \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP            \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_CHECK_FALSE()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define CHECK_FALSE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START;                \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START;                    \
        DOCTEST_CHECK_FALSE(__VA_ARGS__)                                   \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP                  \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_CHECK_EQ()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define CHECK_EQ(...) /** NOLINT(cppcoreguidelines-macro-usage) **/         \
                          /** NOLINTNEXTLINE(cppcoreguidelines-macro-usage) **/ \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START;                     \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START;                         \
        DOCTEST_CHECK_EQ(__VA_ARGS__)                                           \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP                       \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_CHECK_NE()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define CHECK_NE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START;             \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START;                 \
        DOCTEST_CHECK_NE(__VA_ARGS__)                                   \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP               \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_CHECK_GT()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define CHECK_GT(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START;             \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START;                 \
        DOCTEST_CHECK_GT(__VA_ARGS__)                                   \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP               \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_CHECK_LT()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define CHECK_LT(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START;             \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START;                 \
        DOCTEST_CHECK_LT(__VA_ARGS__)                                   \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP               \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_CHECK_GE()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define CHECK_GE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START;             \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START;                 \
        DOCTEST_CHECK_GE(__VA_ARGS__)                                   \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP               \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_CHECK_LE()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define CHECK_LE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START;             \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START;                 \
        DOCTEST_CHECK_LE(__VA_ARGS__)                                   \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP               \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP

    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_REQUIRE()`, this just ignores particular warnings coming from
    /// doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define REQUIRE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START;            \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START;                \
        DOCTEST_REQUIRE(__VA_ARGS__)                                   \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP              \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_REQUIRE_FALSE()`, this just ignores particular warnings
    /// coming from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define REQUIRE_FALSE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START;                  \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START;                      \
        DOCTEST_REQUIRE_FALSE(__VA_ARGS__)                                   \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP                    \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_REQUIRE_EQ()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define REQUIRE_EQ(...) /** NOLINT(cppcoreguidelines-macro-usage) **/         \
                            /** NOLINTNEXTLINE(cppcoreguidelines-macro-usage) **/ \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START;                       \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START;                           \
        DOCTEST_REQUIRE_EQ(__VA_ARGS__)                                           \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP                         \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_REQUIRE_NE()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define REQUIRE_NE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START;               \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START;                   \
        DOCTEST_REQUIRE_NE(__VA_ARGS__)                                   \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP                 \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_REQUIRE_GT()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define REQUIRE_GT(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START;               \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START;                   \
        DOCTEST_REQUIRE_GT(__VA_ARGS__)                                   \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP                 \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_REQUIRE_LT()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define REQUIRE_LT(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START;               \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START;                   \
        DOCTEST_REQUIRE_LT(__VA_ARGS__)                                   \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP                 \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_REQUIRE_GE()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define REQUIRE_GE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START;               \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START;                   \
        DOCTEST_REQUIRE_GE(__VA_ARGS__)                                   \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP                 \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_REQUIRE_LE()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define REQUIRE_LE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START;               \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START;                   \
        DOCTEST_REQUIRE_LE(__VA_ARGS__)                                   \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP                 \
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP

#else
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_CHECK()`, this just ignores particular warnings coming from
    /// doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define CHECK(...)       /** NOLINT(cppcoreguidelines-macro-usage) **/ hyperion::ignore(__VA_ARGS__)
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_CHECK_FALSE()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define CHECK_FALSE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        hyperion::ignore(__VA_ARGS__)
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_CHECK_EQ()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define CHECK_EQ(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        hyperion::ignore(__VA_ARGS__)
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_CHECK_NE()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define CHECK_NE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        hyperion::ignore(__VA_ARGS__)
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_CHECK_GT()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define CHECK_GT(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        hyperion::ignore(__VA_ARGS__)
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_CHECK_LT()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define CHECK_LT(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        hyperion::ignore(__VA_ARGS__)
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_CHECK_GE()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define CHECK_GE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        hyperion::ignore(__VA_ARGS__)
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_CHECK_LE()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define CHECK_LE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        hyperion::ignore(__VA_ARGS__)

    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_REQUIRE()`, this just ignores particular warnings coming from
    /// doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define REQUIRE(...)       /** NOLINT(cppcoreguidelines-macro-usage) **/ hyperion::ignore(__VA_ARGS__)
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_REQUIRE_FALSE()`, this just ignores particular warnings
    /// coming from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define REQUIRE_FALSE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        hyperion::ignore(__VA_ARGS__)
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_REQUIRE_EQ()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define REQUIRE_EQ(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        hyperion::ignore(__VA_ARGS__)
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_REQUIRE_NE()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define REQUIRE_NE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        hyperion::ignore(__VA_ARGS__)
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_REQUIRE_GT()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define REQUIRE_GT(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        hyperion::ignore(__VA_ARGS__)
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_REQUIRE_LT()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define REQUIRE_LT(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        hyperion::ignore(__VA_ARGS__)
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_REQUIRE_GE()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define REQUIRE_GE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        hyperion::ignore(__VA_ARGS__)
    /// @brief Use to do a doctest test assertion.
    /// Usage is identical to `DOCTEST_REQUIRE_LE()`, this just ignores particular warnings coming
    /// from doctest.
    /// @ingroup defines
    /// @headerfile hyperion/platform/testing.h
    #define REQUIRE_LE(...) /** NOLINT(cppcoreguidelines-macro-usage) **/ \
        hyperion::ignore(__VA_ARGS__)
#endif // HYPERION_ENABLE_TESTING && !defined(DOCTEST_CONFIG_DISABLE)

#endif // HYPERION_PLATFORM_TESTING_H
