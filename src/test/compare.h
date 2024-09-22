/// @file compare.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Tests for safe comparison functions
/// @version 0.5.2
/// @date 2024-09-22
///
/// MIT License
/// @copyright Copyright (c) 2024 Braxton Salyer <braxtonsalyer@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.

#ifndef HYPERION_PLATFORM_COMPARE_TESTS_H
#define HYPERION_PLATFORM_COMPARE_TESTS_H

#include <hyperion/platform.h>
#include <hyperion/platform/compare.h>
#include <hyperion/platform/def.h>
#include <hyperion/platform/types.h>

#include <boost/ut.hpp>

#include <cmath>
#include <cstdlib>
#include <limits>

namespace hyperion::_test::platform::compare {

    // NOLINTNEXTLINE(google-build-using-namespace)
    using namespace boost::ut;
    // NOLINTNEXTLINE(google-build-using-namespace)
    using namespace hyperion::platform::compare;

    // NOLINTNEXTLINE(cert-err58-cpp)
    static const suite<"hyperion::platform::compare"> compare_tests = [] {
        static constexpr auto f32_epsilon = std::numeric_limits<f32>::epsilon();
        static constexpr auto f64_epsilon = std::numeric_limits<f64>::epsilon();
        static constexpr auto fmax_epsilon = std::numeric_limits<fmax>::epsilon();

        static constexpr auto custom_absolute_epsilon
            = make_epsilon<EpsilonType::Absolute>(0.001_f64);
        static constexpr auto custom_relative_epsilon
            = make_epsilon<EpsilonType::Relative>(0.1_f64);

        struct non_arithmetic {
            i32 val;

#if HYPERION_PLATFORM_STD_LIB_HAS_COMPARE

    // clang 15 weirdly gives a zero as nullptr constant warning from this defaulted
    // spaceship operator
    #if HYPERION_PLATFORM_COMPILER_IS_CLANG && __clang_major__ == 15

        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"

    #endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

            auto operator<=>(const non_arithmetic& rhs) const noexcept -> std::strong_ordering
                // NOLINTNEXTLINE(*-use-nullptr): false positive from clang-tidy 15 as well
                = default;

    #if HYPERION_PLATFORM_COMPILER_IS_CLANG && __clang_major == 15

        #pragma GCC diagnostic pop

    #endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

#else
            auto operator==(const non_arithmetic& rhs) const noexcept -> bool = default;

            auto operator<(const non_arithmetic& rhs) const noexcept -> bool {
                return val < rhs.val;
            }

            auto operator<=(const non_arithmetic& rhs) const noexcept -> bool {
                return val <= rhs.val;
            }

            auto operator>(const non_arithmetic& rhs) const noexcept -> bool {
                return val > rhs.val;
            }

            auto operator>=(const non_arithmetic& rhs) const noexcept -> bool {
                return val >= rhs.val;
            }
#endif // HYPERION_PLATFORM_STD_LIB_HAS_COMPARE
        };

        "equality_compare"_test = [] {
            "equal_integers_are_equal"_test = [] {
                expect(that % equality_compare(1, 1));
            };

            "inequal_integers_are_not_equal"_test = [] {
                expect(that % not equality_compare(1, 2));
            };

            "integer_equivalent_floats_are_equal"_test = [] {
                auto result = equality_compare(1, 1.0_f32);
                expect(that % result);
                expect(that % equality_compare(1, 1.0_f64));
                expect(that % equality_compare(1, 1.0_fmax));
                expect(that % equality_compare(1'000_i32, 1'000.0_f32));
                expect(that % equality_compare(1'000_i32, 1'000.0_f64));
                expect(that % equality_compare(1'000_i32, 1'000.0_fmax));
            };

            "noninteger_equivalent_floats_are_not_equal"_test = [] {
                expect(that % not equality_compare(1, 2.0_f32));
                expect(that % not equality_compare(1, 2.0_f64));
                expect(that % not equality_compare(1, 2.0_fmax));
                expect(that % not equality_compare(1'000_i32, 1'001.0_f32));
                expect(that % not equality_compare(1'000_i32, 1'001.0_f64));
                expect(that % not equality_compare(1'000_i32, 1'001.0_fmax));
            };

            "equivalent_floats_are_equal"_test = [] {
                expect(that % equality_compare(1.0_f32, 1.0_f32));
                expect(that % equality_compare(1.0_f64, 1.0_f64));
                expect(that % equality_compare(1.0_fmax, 1.0_fmax));
            };

            "nonequivalent_floats_are_not_equal"_test = [] {
                expect(that % not equality_compare(1.0_f32, 2.0_f32));
                expect(that % not equality_compare(1.0_f64, 2.0_f64));
                expect(that % not equality_compare(1.0_fmax, 2.0_fmax));
            };

            "floats_differing_by_only_1_epsilon_are_equal"_test = [] {
                expect(that % equality_compare(1.0_f32, 1.0_f32 + f32_epsilon));
                expect(that % equality_compare(1.0_f64, 1.0_f64 + f64_epsilon));
                expect(that % equality_compare(1.0_fmax, 1.0_fmax + fmax_epsilon));
            };

            "large_floats_with_equal_resolution_are_approximately_equal"_test = [] {
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % equality_compare(1.0e10F, 10'000'000'001.0_f32));
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % equality_compare(1.0e16, 10'000'000'000'000'001.0));
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % equality_compare(1.0e22L, 10'000'000'000'000'000'000'001.0L));
            };

            "significantly_different_large_floats_are_not_approximately_equal"_test = [] {
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % not equality_compare(1.0e10F, 10'000'001'000.0_f32));
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % not equality_compare(1.0e10, 10'000'001'000.0_f64));
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % not equality_compare(1.0e10L, 10'000'001'000.0_fmax));
            };

            "comparisons_are_mathematically_consistent"_test = [] {
                "values_calculated_from_addition_and_subtraction_compare_correctly"_test = [] {
                    const auto one_tenth = 0.1_f64;
                    const auto two_tenths = 0.2_f64;
                    const auto three_tenths = 0.3_f64;
                    const auto calculated = one_tenth + two_tenths;

                    expect(that % equality_compare(three_tenths, calculated));
                    expect(that % equality_compare(three_tenths - calculated, 0.0_f64));
                };

                "negated_values_compare_correctly"_test = [] {
                    expect(that % equality_compare(-0.0_f64, 0.0_f64));
                    expect(that % not equality_compare(-1.0_f64, 1.0_f64));
                };

                "identity_operations_compare_correctly"_test = [] {
                    const auto test_value = 1.23456_f64;
                    expect(that
                           % equality_compare(std::abs(test_value - 0.0_f64)
                                                  / (std::abs(test_value) + 0.0_f64),
                                              1.0_f64));
                };

                "limits_compare_correctly"_test = [] {
                    expect(that
                           % equality_compare(std::numeric_limits<f32>::max(),
                                              std::numeric_limits<f32>::max()));
                    expect(that
                           % equality_compare(std::numeric_limits<f64>::max(),
                                              std::numeric_limits<f64>::max()));
                    expect(that
                           % equality_compare(std::numeric_limits<fmax>::max(),
                                              std::numeric_limits<fmax>::max()));
                    expect(that
                           % not equality_compare(std::numeric_limits<f32>::max(),
                                                  std::numeric_limits<f64>::max()));
                    expect(that
                           % not equality_compare(std::numeric_limits<f32>::max(),
                                                  std::numeric_limits<fmax>::max()));

                    expect(that
                           % equality_compare(std::numeric_limits<f32>::min(),
                                              std::numeric_limits<f32>::min()));
                    expect(that
                           % equality_compare(std::numeric_limits<f64>::min(),
                                              std::numeric_limits<f64>::min()));
                    expect(that
                           % equality_compare(std::numeric_limits<fmax>::min(),
                                              std::numeric_limits<fmax>::min()));
                    expect(that
                           % equality_compare(std::numeric_limits<f32>::min(),
                                              std::numeric_limits<f32>::min()));
                    expect(that
                           % equality_compare(std::numeric_limits<f32>::min(),
                                              std::numeric_limits<f64>::min()));
                    expect(that
                           % equality_compare(std::numeric_limits<f32>::min(),
                                              std::numeric_limits<fmax>::min()));

                    expect(that
                           % equality_compare(std::numeric_limits<f32>::denorm_min(),
                                              std::numeric_limits<f32>::denorm_min()));
                    expect(that
                           % equality_compare(std::numeric_limits<f64>::denorm_min(),
                                              std::numeric_limits<f64>::denorm_min()));
                    expect(that
                           % equality_compare(std::numeric_limits<fmax>::denorm_min(),
                                              std::numeric_limits<fmax>::denorm_min()));
                    expect(that
                           % equality_compare(std::numeric_limits<f32>::denorm_min(),
                                              std::numeric_limits<f32>::denorm_min()));
                    expect(that
                           % equality_compare(std::numeric_limits<f32>::denorm_min(),
                                              std::numeric_limits<f64>::denorm_min()));
                    expect(that
                           % equality_compare(std::numeric_limits<f32>::denorm_min(),
                                              std::numeric_limits<fmax>::denorm_min()));

                    expect(that % not equality_compare(std::numeric_limits<f32>::min(), 1.0));
                    expect(that % not equality_compare(std::numeric_limits<f64>::min(), 1.0));
                    expect(that % not equality_compare(std::numeric_limits<fmax>::min(), 1.0));
                };

                "math_results_compare_correctly"_test = [] {
                    const auto sine1 = std::sin(0.0_f64);
                    const auto sine2 = std::sin(f64_epsilon);
                    expect(that % equality_compare(sine1, 0.0));
                    expect(that % equality_compare(sine2, 0.0));
                    expect(that % equality_compare(sine1, sine2));
                };
            };

            "non_arithmetic_types_compare_correctly"_test = [] {
                const auto val1 = non_arithmetic{0_i32};
                const auto val2 = non_arithmetic{1_i32};
                const auto val3 = non_arithmetic{0_i32};

                expect(that % equality_compare(val1, val1));
                expect(that % equality_compare(val1, val3));
                expect(that % not equality_compare(val1, val2));
            };

            "custom_absolute_epsilon"_test = [] {
                expect(that % equality_compare(1.0_f64, 1.0_f64, custom_absolute_epsilon));
                expect(that
                       % equality_compare(1.0_f64,
                                          1.0_f64 + custom_absolute_epsilon.value(),
                                          custom_absolute_epsilon));
                expect(that
                       % not equality_compare(1.0_f64,
                                              1.0_f64 + custom_absolute_epsilon.value()
                                                  + custom_absolute_epsilon.value(),
                                              custom_absolute_epsilon));
            };

            "custom_relative_epsilon"_test = [] {
                expect(that % equality_compare(1.0_f64, 1.0_f64, custom_relative_epsilon));
                expect(that
                       % equality_compare(1.0_f64, 1.0_f64 + 0.1_f64, custom_relative_epsilon));
                expect(that
                       % not equality_compare(1.0_f64, 1.0_f64 + 0.2_f64, custom_relative_epsilon));

                expect(that
                       % equality_compare(2.0_f64, 2.0_f64 + 0.1_f64, custom_relative_epsilon));
                expect(that
                       % equality_compare(2.0_f64, 2.0_f64 + 0.2_f64, custom_relative_epsilon));
                expect(that
                       % not equality_compare(2.0_f64, 2.0_f64 + 0.3_f64, custom_relative_epsilon));
            };
        };

        "inequality_compare"_test = [] {
            "equal_integers_are_equal"_test = [] {
                expect(that % not inequality_compare(1, 1));
            };

            "inequal_integers_are_not_equal"_test = [] {
                expect(that % inequality_compare(1, 2));
            };

            "integer_equivalent_floats_are_equal"_test = [] {
                expect(that % not inequality_compare(1, 1.0_f32));
                expect(that % not inequality_compare(1, 1.0_f64));
                expect(that % not inequality_compare(1, 1.0_fmax));
                expect(that % not inequality_compare(1'000_i32, 1'000.0_f32));
                expect(that % not inequality_compare(1'000_i32, 1'000.0_f64));
                expect(that % not inequality_compare(1'000_i32, 1'000.0_fmax));
            };

            "noninteger_equivalent_floats_are_not_equal"_test = [] {
                expect(that % inequality_compare(1, 2.0_f32));
                expect(that % inequality_compare(1, 2.0_f64));
                expect(that % inequality_compare(1, 2.0_fmax));
                expect(that % inequality_compare(1'000_i32, 1'001.0_f32));
                expect(that % inequality_compare(1'000_i32, 1'001.0_f64));
                expect(that % inequality_compare(1'000_i32, 1'001.0_fmax));
            };

            "equivalent_floats_are_equal"_test = [] {
                expect(that % not inequality_compare(1.0_f32, 1.0_f32));
                expect(that % not inequality_compare(1.0_f64, 1.0_f64));
                expect(that % not inequality_compare(1.0_fmax, 1.0_fmax));
            };

            "nonequivalent_floats_are_not_equal"_test = [] {
                expect(that % inequality_compare(1.0_f32, 2.0_f32));
                expect(that % inequality_compare(1.0_f64, 2.0_f64));
                expect(that % inequality_compare(1.0_fmax, 2.0_fmax));
            };

            "floats_differing_by_only_1_epsilon_are_equal"_test = [] {
                expect(that % not inequality_compare(1.0_f32, 1.0_f32 + f32_epsilon));
                expect(that % not inequality_compare(1.0_f64, 1.0_f64 + f64_epsilon));
                expect(that % not inequality_compare(1.0_fmax, 1.0_fmax + fmax_epsilon));
            };

            "large_floats_with_equal_resolution_are_approximately_equal"_test = [] {
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % not inequality_compare(1.0e10F, 10'000'000'001.0_f32));
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % not inequality_compare(1.0e16, 10'000'000'000'000'001.0));
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % not inequality_compare(1.0e22L, 10'000'000'000'000'000'000'001.0L));
            };

            "significantly_different_large_floats_are_not_approximately_equal"_test = [] {
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % inequality_compare(1.0e10F, 10'000'001'000.0_f32));
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % inequality_compare(1.0e10, 10'000'001'000.0_f64));
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % inequality_compare(1.0e10L, 10'000'001'000.0_fmax));
            };

            "comparisons_are_mathematically_consistent"_test = [] {
                "values_calculated_from_addition_and_subtraction_compare_correctly"_test = [] {
                    const auto one_tenth = 0.1_f64;
                    const auto two_tenths = 0.2_f64;
                    const auto three_tenths = 0.3_f64;
                    const auto calculated = one_tenth + two_tenths;

                    expect(that % not inequality_compare(three_tenths, calculated));
                    expect(that % not inequality_compare(three_tenths - calculated, 0.0_f64));
                };

                "negated_values_compare_correctly"_test = [] {
                    expect(that % not inequality_compare(-0.0_f64, 0.0_f64));
                    expect(that % inequality_compare(-1.0_f64, 1.0_f64));
                };

                "identity_operations_compare_correctly"_test = [] {
                    const auto test_value = 1.23456_f64;
                    expect(that
                           % not inequality_compare(std::abs(test_value - 0.0_f64)
                                                        / (std::abs(test_value) + 0.0_f64),
                                                    1.0_f64));
                };

                "limits_compare_correctly"_test = [] {
                    expect(that
                           % not inequality_compare(std::numeric_limits<f32>::max(),
                                                    std::numeric_limits<f32>::max()));
                    expect(that
                           % not inequality_compare(std::numeric_limits<f64>::max(),
                                                    std::numeric_limits<f64>::max()));
                    expect(that
                           % not inequality_compare(std::numeric_limits<fmax>::max(),
                                                    std::numeric_limits<fmax>::max()));
                    expect(that
                           % inequality_compare(std::numeric_limits<f32>::max(),
                                                std::numeric_limits<f64>::max()));
                    expect(that
                           % inequality_compare(std::numeric_limits<f32>::max(),
                                                std::numeric_limits<fmax>::max()));

                    expect(that
                           % not inequality_compare(std::numeric_limits<f32>::min(),
                                                    std::numeric_limits<f32>::min()));
                    expect(that
                           % not inequality_compare(std::numeric_limits<f64>::min(),
                                                    std::numeric_limits<f64>::min()));
                    expect(that
                           % not inequality_compare(std::numeric_limits<fmax>::min(),
                                                    std::numeric_limits<fmax>::min()));
                    expect(that
                           % not inequality_compare(std::numeric_limits<f32>::min(),
                                                    std::numeric_limits<f32>::min()));
                    expect(that
                           % not inequality_compare(std::numeric_limits<f32>::min(),
                                                    std::numeric_limits<f64>::min()));
                    expect(that
                           % not inequality_compare(std::numeric_limits<f32>::min(),
                                                    std::numeric_limits<fmax>::min()));

                    expect(that
                           % not inequality_compare(std::numeric_limits<f32>::denorm_min(),
                                                    std::numeric_limits<f32>::denorm_min()));
                    expect(that
                           % not inequality_compare(std::numeric_limits<f64>::denorm_min(),
                                                    std::numeric_limits<f64>::denorm_min()));
                    expect(that
                           % not inequality_compare(std::numeric_limits<fmax>::denorm_min(),
                                                    std::numeric_limits<fmax>::denorm_min()));
                    expect(that
                           % not inequality_compare(std::numeric_limits<f32>::denorm_min(),
                                                    std::numeric_limits<f32>::denorm_min()));
                    expect(that
                           % not inequality_compare(std::numeric_limits<f32>::denorm_min(),
                                                    std::numeric_limits<f64>::denorm_min()));
                    expect(that
                           % not inequality_compare(std::numeric_limits<f32>::denorm_min(),
                                                    std::numeric_limits<fmax>::denorm_min()));

                    expect(that % inequality_compare(std::numeric_limits<f32>::min(), 1.0));
                    expect(that % inequality_compare(std::numeric_limits<f64>::min(), 1.0));
                    expect(that % inequality_compare(std::numeric_limits<fmax>::min(), 1.0));
                };

                "math_results_compare_correctly"_test = [] {
                    const auto sine1 = std::sin(0.0_f64);
                    const auto sine2 = std::sin(f64_epsilon);
                    expect(that % not inequality_compare(sine1, 0.0));
                    expect(that % not inequality_compare(sine2, 0.0));
                    expect(that % not inequality_compare(sine1, sine2));
                };
            };

            "non_arithmetic_types_compare_correctly"_test = [] {
                const auto val1 = non_arithmetic{0_i32};
                const auto val2 = non_arithmetic{1_i32};
                const auto val3 = non_arithmetic{0_i32};

                expect(that % inequality_compare(val1, val2));
                expect(that % not inequality_compare(val1, val1));
                expect(that % not inequality_compare(val1, val3));
            };

            "custom_absolute_epsilon"_test = [] {
                expect(that % not inequality_compare(1.0_f64, 1.0_f64, custom_absolute_epsilon));
                expect(that
                       % not inequality_compare(1.0_f64,
                                                1.0_f64 + custom_absolute_epsilon.value(),
                                                custom_absolute_epsilon));
                expect(that
                       % inequality_compare(1.0_f64,
                                            1.0_f64 + custom_absolute_epsilon.value()
                                                + custom_absolute_epsilon.value(),
                                            custom_absolute_epsilon));
            };

            "custom_relative_epsilon"_test = [] {
                expect(that % not inequality_compare(1.0_f64, 1.0_f64, custom_relative_epsilon));
                expect(
                    that
                    % not inequality_compare(1.0_f64, 1.0_f64 + 0.1_f64, custom_relative_epsilon));
                expect(that
                       % inequality_compare(1.0_f64, 1.0_f64 + 0.2_f64, custom_relative_epsilon));

                expect(
                    that
                    % not inequality_compare(2.0_f64, 2.0_f64 + 0.1_f64, custom_relative_epsilon));
                expect(
                    that
                    % not inequality_compare(2.0_f64, 2.0_f64 + 0.2_f64, custom_relative_epsilon));
                expect(that
                       % inequality_compare(2.0_f64, 2.0_f64 + 0.3_f64, custom_relative_epsilon));
            };
        };

        "less_than"_test = [] {
            "equal_integers_are_not_less_than"_test = [] {
                expect(that % not less_than_compare(1_i32, 1_i32));
            };

            "less_than_integers_are_less_than"_test = [] {
                expect(that % less_than_compare(1_i32, 2_i32));
                expect(that % less_than_compare(3_i32, 4_i32));
            };

            "greater_than_integers_are_not_less_than"_test = [] {
                expect(that % not less_than_compare(2_i32, 1_i32));
            };

            "less_than_integers_are_less_than_very_near_floats"_test = [] {
                expect(that % less_than_compare(1_i32, 1.01_f32));
                expect(that % less_than_compare(1'000_i32, 1001.0_f32));
            };

            "equal_floats_are_not_less_than"_test = [] {
                expect(that % not less_than_compare(1.0_f32, 1.0_f32));
                expect(that % not less_than_compare(1.0_f64, 1.0_f64));
                expect(that % not less_than_compare(1.0_fmax, 1.0_fmax));
            };

            "less_than_floats_are_less_than"_test = [] {
                expect(that % less_than_compare(1.0_f32, 2.0_f32));
                expect(that % less_than_compare(1.0_f64, 2.0_f64));
                expect(that % less_than_compare(1.0_fmax, 2.0_fmax));
            };

            "floats_differing_by_only_1_epsilon_are_not_less_than"_test = [] {
                expect(that % not less_than_compare(1.0_f32, 1.0_f32 + f32_epsilon));
                expect(that % not less_than_compare(1.0_f64, 1.0_f64 + f64_epsilon));
                expect(that % not less_than_compare(1.0_fmax, 1.0_fmax + fmax_epsilon));
            };

            "floats_differing_by_more_than_1_epsilon_are_not_less_than"_test = [] {
                expect(that % less_than_compare(1.0_f32, 1.0_f32 + f32_epsilon + f32_epsilon));
                expect(that % less_than_compare(1.0_f64, 1.0_f64 + f64_epsilon + f64_epsilon));
                expect(that % less_than_compare(1.0_fmax, 1.0_fmax + fmax_epsilon + fmax_epsilon));
            };

            "floats_near_resolution_limits_compare_correctly"_test = [] {
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % less_than_compare(1.0e10F, 10'000'001'000.0_f32));
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % less_than_compare(1.0e16, 10'000'000'000'001'000.0));
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % less_than_compare(1.0e22L, 10'000'000'000'000'010'000'000.0L));

                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % not less_than_compare(1.0e10F, 10'000'000'001.0_f32));
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % not less_than_compare(1.0e16, 10'000'000'000'000'001.0));
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % not less_than_compare(1.0e22L, 10'000'000'000'000'000'000'001.0L));
            };

            "comparisons_are_mathematically_consistent"_test = [] {
                "values_calculated_from_addition_and_subtraction_compare_correctly"_test = [] {
                    const auto one_tenth = 0.1_f64;
                    const auto two_tenths = 0.2_f64;
                    const auto three_tenths = 0.3_f64;
                    const auto calculated = one_tenth + two_tenths;

                    expect(that % less_than_compare(one_tenth, two_tenths));
                    expect(that % less_than_compare(two_tenths, three_tenths));
                    expect(that % less_than_compare(two_tenths, calculated));
                    expect(that % less_than_compare(one_tenth, three_tenths));
                    expect(that % less_than_compare(one_tenth, calculated));
                    expect(that % not less_than_compare(three_tenths, calculated));
                    expect(that % not less_than_compare(three_tenths - calculated, 0.0_f64));
                };

                "negated_values_compare_correctly"_test = [] {
                    expect(that % not less_than_compare(-0.0_f64, 0.0_f64));
                    expect(that % less_than_compare(-1.0_f64, 1.0_f64));
                    expect(that % not less_than_compare(-1.0_f64, -1.0_f64));
                    expect(that % not less_than_compare(1.0_f64, -1.0_f64));
                };

                "identity_operations_compare_correctly"_test = [] {
                    const auto test_value = 1.23456_f64;
                    expect(that
                           % not less_than_compare(std::abs(test_value - 0.0_f64)
                                                       / (std::abs(test_value) + 0.0_f64),
                                                   1.0_f64));
                };
            };

            "non_arithmetic_types_compare_correctly"_test = [] {
                const auto val1 = non_arithmetic{0_i32};
                const auto val2 = non_arithmetic{1_i32};
                const auto val3 = non_arithmetic{0_i32};

                expect(that % less_than_compare(val1, val2));
                expect(that % not less_than_compare(val1, val1));
                expect(that % not less_than_compare(val1, val3));
            };

            "custom_absolute_epsilon"_test = [] {
                expect(that % not less_than_compare(1.0_f64, 1.0_f64, custom_absolute_epsilon));
                expect(that
                       % not less_than_compare(1.0_f64,
                                               1.0_f64 + custom_absolute_epsilon.value(),
                                               custom_absolute_epsilon));
                expect(that
                       % less_than_compare(1.0_f64,
                                           1.0_f64 + custom_absolute_epsilon.value()
                                               + custom_absolute_epsilon.value(),
                                           custom_absolute_epsilon));
            };

            "custom_relative_epsilon"_test = [] {
                expect(that % not less_than_compare(1.0_f64, 1.0_f64, custom_relative_epsilon));
                expect(
                    that
                    % not less_than_compare(1.0_f64, 1.0_f64 + 0.1_f64, custom_relative_epsilon));
                expect(that
                       % less_than_compare(1.0_f64, 1.0_f64 + 0.2_f64, custom_relative_epsilon));

                expect(
                    that
                    % not less_than_compare(2.0_f64, 2.0_f64 + 0.1_f64, custom_relative_epsilon));
                expect(
                    that
                    % not less_than_compare(2.0_f64, 2.0_f64 + 0.2_f64, custom_relative_epsilon));
                expect(that
                       % less_than_compare(2.0_f64, 2.0_f64 + 0.3_f64, custom_relative_epsilon));
            };
        };

        "less_than_or_equal"_test = [] {
            "equal_integers_are_less_than_or_equal"_test = [] {
                expect(that % less_than_or_equal_compare(1_i32, 1_i32));
            };

            "less_than_integers_are_less_than_or_equal"_test = [] {
                expect(that % less_than_or_equal_compare(1_i32, 2_i32));
                expect(that % less_than_or_equal_compare(3_i32, 4_i32));
            };

            "greater_than_integers_are_not_less_than_or_equal"_test = [] {
                expect(that % not less_than_or_equal_compare(2_i32, 1_i32));
            };

            "less_than_integers_are_less_than_or_equal_very_near_floats"_test = [] {
                expect(that % less_than_or_equal_compare(1_i32, 1.01_f32));
                expect(that % less_than_or_equal_compare(1'000_i32, 1001.0_f32));
            };

            "equal_floats_are_less_than_or_equal"_test = [] {
                expect(that % less_than_or_equal_compare(1.0_f32, 1.0_f32));
                expect(that % less_than_or_equal_compare(1.0_f64, 1.0_f64));
                expect(that % less_than_or_equal_compare(1.0_fmax, 1.0_fmax));
            };

            "less_than_floats_are_less_than_or_equal"_test = [] {
                expect(that % less_than_or_equal_compare(1.0_f32, 2.0_f32));
                expect(that % less_than_or_equal_compare(1.0_f64, 2.0_f64));
                expect(that % less_than_or_equal_compare(1.0_fmax, 2.0_fmax));
            };

            "floats_differing_by_only_1_epsilon_are_less_than_or_equal"_test = [] {
                expect(that % less_than_or_equal_compare(1.0_f32, 1.0_f32 + f32_epsilon));
                expect(that % less_than_or_equal_compare(1.0_f64, 1.0_f64 + f64_epsilon));
                expect(that % less_than_or_equal_compare(1.0_fmax, 1.0_fmax + fmax_epsilon));
            };

            "floats_differing_by_more_than_1_epsilon_are_less_than_or_equal"_test = [] {
                expect(that
                       % less_than_or_equal_compare(1.0_f32, 1.0_f32 + f32_epsilon + f32_epsilon));
                expect(that
                       % less_than_or_equal_compare(1.0_f64, 1.0_f64 + f64_epsilon + f64_epsilon));
                expect(
                    that
                    % less_than_or_equal_compare(1.0_fmax, 1.0_fmax + fmax_epsilon + fmax_epsilon));
            };

            "floats_near_resolution_limits_compare_correctly"_test = [] {
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % less_than_or_equal_compare(1.0e10F, 10'000'001'000.0_f32));
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % less_than_or_equal_compare(1.0e16, 10'000'000'000'001'000.0));
                expect(that
                       // NOLINTNEXTLINE(*-magic-numbers)
                       % less_than_or_equal_compare(1.0e22L, 10'000'000'000'000'000'001'000.0L));

                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % less_than_or_equal_compare(1.0e10F, 10'000'000'001.0_f32));
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % less_than_or_equal_compare(1.0e16, 10'000'000'000'000'001.0));
                expect(that
                       // NOLINTNEXTLINE(*-magic-numbers)
                       % less_than_or_equal_compare(1.0e22L, 10'000'000'000'000'000'000'001.0L));
            };

            "comparisons_are_mathematically_consistent"_test = [] {
                "values_calculated_from_addition_and_subtraction_compare_correctly"_test = [] {
                    const auto one_tenth = 0.1_f64;
                    const auto two_tenths = 0.2_f64;
                    const auto three_tenths = 0.3_f64;
                    const auto calculated = one_tenth + two_tenths;

                    expect(that % less_than_or_equal_compare(one_tenth, two_tenths));
                    expect(that % less_than_or_equal_compare(two_tenths, three_tenths));
                    expect(that % less_than_or_equal_compare(two_tenths, calculated));
                    expect(that % less_than_or_equal_compare(one_tenth, three_tenths));
                    expect(that % less_than_or_equal_compare(one_tenth, calculated));
                    expect(that % less_than_or_equal_compare(three_tenths, calculated));
                    expect(that % less_than_or_equal_compare(three_tenths - calculated, 0.0_f64));
                };

                "negated_values_compare_correctly"_test = [] {
                    expect(that % less_than_or_equal_compare(-0.0_f64, 0.0_f64));
                    expect(that % less_than_or_equal_compare(-1.0_f64, 1.0_f64));
                    expect(that % less_than_or_equal_compare(-1.0_f64, -1.0_f64));
                    expect(that % not less_than_or_equal_compare(1.0_f64, -1.0_f64));
                };

                "identity_operations_compare_correctly"_test = [] {
                    const auto test_value = 1.23456_f64;
                    expect(that
                           % less_than_or_equal_compare(std::abs(test_value - 0.0_f64)
                                                            / (std::abs(test_value) + 0.0_f64),
                                                        1.0_f64));
                };
            };

            "non_arithmetic_types_compare_correctly"_test = [] {
                const auto val1 = non_arithmetic{0_i32};
                const auto val2 = non_arithmetic{1_i32};
                const auto val3 = non_arithmetic{0_i32};

                expect(that % less_than_or_equal_compare(val1, val2));
                expect(that % less_than_or_equal_compare(val1, val1));
                expect(that % less_than_or_equal_compare(val1, val3));
            };

            "custom_absolute_epsilon"_test = [] {
                expect(that
                       % less_than_or_equal_compare(1.0_f64, 1.0_f64, custom_absolute_epsilon));
                expect(that
                       % less_than_or_equal_compare(1.0_f64,
                                                    1.0_f64 + custom_absolute_epsilon.value(),
                                                    custom_absolute_epsilon));
                expect(that
                       % less_than_or_equal_compare(1.0_f64,
                                                    1.0_f64 + custom_absolute_epsilon.value()
                                                        + custom_absolute_epsilon.value(),
                                                    custom_absolute_epsilon));
                expect(that
                       % not less_than_or_equal_compare(1.1_f64, 1.0_f64, custom_absolute_epsilon));
            };

            "custom_relative_epsilon"_test = [] {
                expect(that
                       % less_than_or_equal_compare(1.0_f64, 1.0_f64, custom_relative_epsilon));
                expect(that
                       % less_than_or_equal_compare(1.0_f64,
                                                    1.0_f64 + 0.1_f64,
                                                    custom_relative_epsilon));
                expect(that
                       % less_than_or_equal_compare(1.0_f64,
                                                    1.0_f64 + 0.2_f64,
                                                    custom_relative_epsilon));
                expect(that
                       % not less_than_or_equal_compare(1.2_f64, 1.0_f64, custom_relative_epsilon));

                expect(that
                       % less_than_or_equal_compare(2.0_f64,
                                                    2.0_f64 + 0.1_f64,
                                                    custom_relative_epsilon));
                expect(that
                       % less_than_or_equal_compare(2.0_f64,
                                                    2.0_f64 + 0.2_f64,
                                                    custom_relative_epsilon));
                expect(that
                       % less_than_or_equal_compare(2.0_f64,
                                                    2.0_f64 + 0.3_f64,
                                                    custom_relative_epsilon));
                expect(that
                       % not less_than_or_equal_compare(2.3_f64, 2.0_f64, custom_relative_epsilon));
            };
        };

        "greater_than"_test = [] {
            "equal_integers_are_not_greater_than"_test = [] {
                expect(that % not greater_than_compare(1_i32, 1_i32));
            };

            "less_than_integers_are_not_greater_than"_test = [] {
                expect(that % not greater_than_compare(1_i32, 2_i32));
                expect(that % not greater_than_compare(3_i32, 4_i32));
            };

            "greater_than_integers_are_greater_than"_test = [] {
                expect(that % greater_than_compare(2_i32, 1_i32));
                expect(that % greater_than_compare(1_i32, 0_i32));
            };

            "greater_than_integers_are_greater_than_very_near_floats"_test = [] {
                expect(that % greater_than_compare(1_i32, 0.9999_f32));
                expect(that % greater_than_compare(1'000_i32, 999.99_f32));
            };

            "equal_floats_are_not_greater_than"_test = [] {
                expect(that % not greater_than_compare(1.0_f32, 1.0_f32));
                expect(that % not greater_than_compare(1.0_f64, 1.0_f64));
                expect(that % not greater_than_compare(1.0_fmax, 1.0_fmax));
            };

            "less_than_floats_are_not_greater_than"_test = [] {
                expect(that % not greater_than_compare(1.0_f32, 2.0_f32));
                expect(that % not greater_than_compare(1.0_f64, 2.0_f64));
                expect(that % not greater_than_compare(1.0_fmax, 2.0_fmax));
            };

            "greater_than_floats_are_greater_than"_test = [] {
                expect(that % greater_than_compare(2.0_f32, 1.0_f32));
                expect(that % greater_than_compare(2.0_f64, 1.0_f64));
                expect(that % greater_than_compare(2.0_fmax, 1.0_fmax));
            };

            "floats_differing_by_only_1_epsilon_are_not_greater_than"_test = [] {
                expect(that % not greater_than_compare(1.0_f32 + f32_epsilon, 1.0_f32));
                expect(that % not greater_than_compare(1.0_f64 + f64_epsilon, 1.0_f64));
                expect(that % not greater_than_compare(1.0_fmax + fmax_epsilon, 1.0_fmax));
            };

            "floats_differing_by_more_than_1_epsilon_are_greater_than"_test = [] {
                expect(that % greater_than_compare(1.0_f32 + f32_epsilon + f32_epsilon, 1.0_f32));
                expect(that % greater_than_compare(1.0_f64 + f64_epsilon + f64_epsilon, 1.0_f64));
                expect(that
                       % greater_than_compare(1.0_fmax + fmax_epsilon + fmax_epsilon, 1.0_fmax));
            };

            "floats_near_resolution_limits_compare_correctly"_test = [] {
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % greater_than_compare(10'000'001'000.0_f32, 1.0e10F));
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % greater_than_compare(10'000'000'000'001'000.0, 1.0e16));
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % greater_than_compare(10'000'000'000'000'010'000'000.0L, 1.0e22L));

                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % not greater_than_compare(1.0e10F, 10'000'000'001.0_f32));
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % not greater_than_compare(1.0e16, 10'000'000'000'000'001.0));
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % not greater_than_compare(1.0e22L, 10'000'000'000'000'000'000'001.0L));
            };

            "comparisons_are_mathematically_consistent"_test = [] {
                "values_calculated_from_addition_and_subtraction_compare_correctly"_test = [] {
                    const auto one_tenth = 0.1_f64;
                    const auto two_tenths = 0.2_f64;
                    const auto three_tenths = 0.3_f64;
                    const auto calculated = one_tenth + two_tenths;

                    expect(that % greater_than_compare(two_tenths, one_tenth));
                    expect(that % greater_than_compare(three_tenths, two_tenths));
                    expect(that % greater_than_compare(calculated, two_tenths));
                    expect(that % greater_than_compare(three_tenths, one_tenth));
                    expect(that % greater_than_compare(calculated, one_tenth));
                    expect(that % not greater_than_compare(three_tenths, calculated));
                    expect(that % not greater_than_compare(three_tenths - calculated, 0.0_f64));
                };

                "negated_values_compare_correctly"_test = [] {
                    expect(that % not greater_than_compare(-0.0_f64, 0.0_f64));
                    expect(that % greater_than_compare(1.0_f64, -1.0_f64));
                    expect(that % not greater_than_compare(-1.0_f64, -1.0_f64));
                    expect(that % not greater_than_compare(-1.0_f64, 1.0_f64));
                };

                "identity_operations_compare_correctly"_test = [] {
                    const auto test_value = 1.23456_f64;
                    expect(that
                           % not greater_than_compare(std::abs(test_value - 0.0_f64)
                                                          / (std::abs(test_value) + 0.0_f64),
                                                      1.0_f64));
                };
            };

            "non_arithmetic_types_compare_correctly"_test = [] {
                const auto val1 = non_arithmetic{0_i32};
                const auto val2 = non_arithmetic{1_i32};
                const auto val3 = non_arithmetic{0_i32};

                expect(that % greater_than_compare(val2, val1));
                expect(that % not greater_than_compare(val1, val1));
                expect(that % not greater_than_compare(val1, val3));
            };

            "custom_absolute_epsilon"_test = [] {
                expect(that % not greater_than_compare(1.0_f64, 1.0_f64, custom_absolute_epsilon));
                expect(that
                       % not greater_than_compare(1.0_f64,
                                                  1.0_f64 + custom_absolute_epsilon.value(),
                                                  custom_absolute_epsilon));
                expect(that
                       % not greater_than_compare(1.0_f64,
                                                  1.0_f64 + custom_absolute_epsilon.value()
                                                      + custom_absolute_epsilon.value(),
                                                  custom_absolute_epsilon));
                expect(that
                       % greater_than_compare(1.0_f64 + custom_absolute_epsilon.value()
                                                  + custom_absolute_epsilon.value(),
                                              1.0_f64,
                                              custom_absolute_epsilon));
            };

            "custom_relative_epsilon"_test = [] {
                expect(that % not greater_than_compare(1.0_f64, 1.0_f64, custom_relative_epsilon));
                expect(that
                       % not greater_than_compare(1.0_f64,
                                                  1.0_f64 + 0.1_f64,
                                                  custom_relative_epsilon));
                expect(that
                       % not greater_than_compare(1.0_f64,
                                                  1.0_f64 + 0.2_f64,
                                                  custom_relative_epsilon));

                expect(that
                       % not greater_than_compare(2.0_f64,
                                                  2.0_f64 + 0.1_f64,
                                                  custom_relative_epsilon));
                expect(that
                       % not greater_than_compare(2.0_f64,
                                                  2.0_f64 + 0.2_f64,
                                                  custom_relative_epsilon));
                expect(that
                       % not greater_than_compare(2.0_f64,
                                                  2.0_f64 + 0.3_f64,
                                                  custom_relative_epsilon));
                expect(that
                       % greater_than_compare(2.0_f64 + 0.3_f64, 2.0_f64, custom_relative_epsilon));
            };
        };

        "greater_than_or_equal"_test = [] {
            "equal_integers_are_greater_than_or_equal"_test = [] {
                expect(that % greater_than_or_equal_compare(1_i32, 1_i32));
            };

            "less_than_integers_are_not_greater_than_or_equal"_test = [] {
                expect(that % not greater_than_or_equal_compare(1_i32, 2_i32));
                expect(that % not greater_than_or_equal_compare(3_i32, 4_i32));
            };

            "greater_than_integers_are_greater_than_or_equal"_test = [] {
                expect(that % greater_than_or_equal_compare(2_i32, 1_i32));
                expect(that % greater_than_or_equal_compare(4_i32, 3_i32));
            };

            "less_than_integers_are_not_greater_than_or_equal_very_near_floats"_test = [] {
                expect(that % not greater_than_or_equal_compare(1_i32, 1.01_f32));
                expect(that % not greater_than_or_equal_compare(1'000_i32, 1001.0_f32));
            };

            "equal_floats_are_greater_than_or_equal"_test = [] {
                expect(that % greater_than_or_equal_compare(1.0_f32, 1.0_f32));
                expect(that % greater_than_or_equal_compare(1.0_f64, 1.0_f64));
                expect(that % greater_than_or_equal_compare(1.0_fmax, 1.0_fmax));
            };

            "less_than_floats_are_not_greater_than_or_equal"_test = [] {
                expect(that % not greater_than_or_equal_compare(1.0_f32, 2.0_f32));
                expect(that % not greater_than_or_equal_compare(1.0_f64, 2.0_f64));
                expect(that % not greater_than_or_equal_compare(1.0_fmax, 2.0_fmax));
            };

            "greater_than_floats_are_greater_than_or_equal"_test = [] {
                expect(that % greater_than_or_equal_compare(2.0_f32, 1.0_f32));
                expect(that % greater_than_or_equal_compare(2.0_f64, 1.0_f64));
                expect(that % greater_than_or_equal_compare(2.0_fmax, 1.0_fmax));
            };

            "floats_differing_by_only_1_epsilon_are_greater_than_or_equal"_test = [] {
                expect(that % greater_than_or_equal_compare(1.0_f32, 1.0_f32 + f32_epsilon));
                expect(that % greater_than_or_equal_compare(1.0_f64, 1.0_f64 + f64_epsilon));
                expect(that % greater_than_or_equal_compare(1.0_fmax, 1.0_fmax + fmax_epsilon));
            };

            "floats_differing_by_more_than_1_epsilon_are_greater_than_or_equal"_test = [] {
                expect(
                    that
                    % greater_than_or_equal_compare(1.0_f32 + f32_epsilon + f32_epsilon, 1.0_f32));
                expect(
                    that
                    % greater_than_or_equal_compare(1.0_f64 + f64_epsilon + f64_epsilon, 1.0_f64));
                expect(that
                       % greater_than_or_equal_compare(1.0_fmax + fmax_epsilon + fmax_epsilon,
                                                       1.0_fmax));
            };

            "floats_near_resolution_limits_compare_correctly"_test = [] {
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % not greater_than_or_equal_compare(1.0e10F, 10'000'001'000.0_f32));
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % not greater_than_or_equal_compare(1.0e16, 10'000'000'000'001'000.0));
                expect(that
                       // NOLINTNEXTLINE(*-magic-numbers)
                       % not greater_than_or_equal_compare(1.0e22L,
                                                           // NOLINTNEXTLINE(*-magic-numbers)
                                                           10'000'000'000'000'010'000'000.0L));

                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % greater_than_or_equal_compare(1.0e10F, 10'000'000'001.0_f32));
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % greater_than_or_equal_compare(1.0e16, 10'000'000'000'000'001.0));
                expect(that
                       // NOLINTNEXTLINE(*-magic-numbers)
                       % greater_than_or_equal_compare(1.0e22L, 10'000'000'000'000'000'000'001.0L));

                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % greater_than_or_equal_compare(10'000'001'000.0_f32, 1.0e10F));
                // NOLINTNEXTLINE(*-magic-numbers)
                expect(that % greater_than_or_equal_compare(10'000'000'000'001'000.0, 1.0e16));
                expect(that
                       // NOLINTNEXTLINE(*-magic-numbers)
                       % greater_than_or_equal_compare(10'000'000'000'000'000'001'000.0L, 1.0e22L));
            };

            "comparisons_are_mathematically_consistent"_test = [] {
                "values_calculated_from_addition_and_subtraction_compare_correctly"_test = [] {
                    const auto one_tenth = 0.1_f64;
                    const auto two_tenths = 0.2_f64;
                    const auto three_tenths = 0.3_f64;
                    const auto calculated = one_tenth + two_tenths;

                    expect(that % greater_than_or_equal_compare(two_tenths, one_tenth));
                    expect(that % greater_than_or_equal_compare(three_tenths, two_tenths));
                    expect(that % greater_than_or_equal_compare(calculated, two_tenths));
                    expect(that % greater_than_or_equal_compare(three_tenths, one_tenth));
                    expect(that % greater_than_or_equal_compare(calculated, one_tenth));
                    expect(that % greater_than_or_equal_compare(calculated, three_tenths));
                    expect(that
                           % greater_than_or_equal_compare(0.0_f64, three_tenths - calculated));
                };

                "negated_values_compare_correctly"_test = [] {
                    expect(that % greater_than_or_equal_compare(-0.0_f64, 0.0_f64));
                    expect(that % greater_than_or_equal_compare(1.0_f64, -1.0_f64));
                    expect(that % greater_than_or_equal_compare(-1.0_f64, -1.0_f64));
                    expect(that % not greater_than_or_equal_compare(-1.0_f64, 1.0_f64));
                };

                "identity_operations_compare_correctly"_test = [] {
                    const auto test_value = 1.23456_f64;
                    expect(that
                           % greater_than_or_equal_compare(std::abs(test_value - 0.0_f64)
                                                               / (std::abs(test_value) + 0.0_f64),
                                                           1.0_f64));
                };
            };

            "non_arithmetic_types_compare_correctly"_test = [] {
                const auto val1 = non_arithmetic{0_i32};
                const auto val2 = non_arithmetic{1_i32};
                const auto val3 = non_arithmetic{0_i32};

                expect(that % greater_than_or_equal_compare(val2, val1));
                expect(that % greater_than_or_equal_compare(val1, val1));
                expect(that % greater_than_or_equal_compare(val1, val3));
            };

            "custom_absolute_epsilon"_test = [] {
                expect(that
                       % greater_than_or_equal_compare(1.0_f64, 1.0_f64, custom_absolute_epsilon));
                expect(that
                       % greater_than_or_equal_compare(1.0_f64,
                                                       1.0_f64 + custom_absolute_epsilon.value(),
                                                       custom_absolute_epsilon));
                expect(that
                       % not greater_than_or_equal_compare(1.0_f64,
                                                           1.0_f64 + custom_absolute_epsilon.value()
                                                               + custom_absolute_epsilon.value(),
                                                           custom_absolute_epsilon));
                expect(that
                       % greater_than_or_equal_compare(1.0_f64 + custom_absolute_epsilon.value()
                                                           + custom_absolute_epsilon.value(),
                                                       1.0_f64,
                                                       custom_absolute_epsilon));
            };

            "custom_relative_epsilon"_test = [] {
                expect(that
                       % greater_than_or_equal_compare(1.0_f64, 1.0_f64, custom_relative_epsilon));
                expect(that
                       % greater_than_or_equal_compare(1.0_f64,
                                                       1.0_f64 + 0.1_f64,
                                                       custom_relative_epsilon));
                expect(that
                       % not greater_than_or_equal_compare(1.0_f64,
                                                           1.0_f64 + 0.2_f64,
                                                           custom_relative_epsilon));

                expect(that
                       % greater_than_or_equal_compare(2.0_f64,
                                                       2.0_f64 + 0.1_f64,
                                                       custom_relative_epsilon));
                expect(that
                       % greater_than_or_equal_compare(2.0_f64,
                                                       2.0_f64 + 0.2_f64,
                                                       custom_relative_epsilon));
                expect(that
                       % not greater_than_or_equal_compare(2.0_f64,
                                                           2.0_f64 + 0.3_f64,
                                                           custom_relative_epsilon));
                expect(that
                       % greater_than_or_equal_compare(2.0_f64 + 0.3_f64,
                                                       2.0_f64,
                                                       custom_relative_epsilon));
            };
        };
    };

    struct not_comparable { };

    static_assert(EqualityComparable<int, int>,
                  "hyperion::platform::compare::EqualityComparable test case 1 failing");
    static_assert(EqualityComparable<int, double>,
                  "hyperion::platform::compare::EqualityComparable test case 2 failing");
    static_assert(!EqualityComparable<int, not_comparable>,
                  "hyperion::platform::compare::EqualityComparable test case 3 failing");

    static_assert(InequalityComparable<int, int>,
                  "hyperion::platform::compare::InequalityComparable test case 1 failing");
    static_assert(InequalityComparable<int, double>,
                  "hyperion::platform::compare::InequalityComparable test case 2 failing");
    static_assert(!InequalityComparable<int, not_comparable>,
                  "hyperion::platform::compare::InequalityComparable test case 3 failing");

    static_assert(LessThanComparable<int, int>,
                  "hyperion::platform::compare::LessThanComparable test case 1 failing");
    static_assert(LessThanComparable<int, double>,
                  "hyperion::platform::compare::LessThanComparable test case 2 failing");
    static_assert(!LessThanComparable<int, not_comparable>,
                  "hyperion::platform::compare::LessThanComparable test case 3 failing");

    static_assert(LessThanOrEqualComparable<int, int>,
                  "hyperion::platform::compare::LessThanOrEqualComparable test case 1 failing");
    static_assert(LessThanOrEqualComparable<int, double>,
                  "hyperion::platform::compare::LessThanOrEqualComparable test case 2 failing");
    static_assert(!LessThanOrEqualComparable<int, not_comparable>,
                  "hyperion::platform::compare::LessThanOrEqualComparable test case 3 failing");

    static_assert(GreaterThanComparable<int, int>,
                  "hyperion::platform::compare::GreaterThanComparable test case 1 failing");
    static_assert(GreaterThanComparable<int, double>,
                  "hyperion::platform::compare::GreaterThanComparable test case 2 failing");
    static_assert(!GreaterThanComparable<int, not_comparable>,
                  "hyperion::platform::compare::GreaterThanComparable test case 3 failing");

    static_assert(GreaterThanOrEqualComparable<int, int>,
                  "hyperion::platform::compare::GreaterThanOrEqualComparable test case 1 failing");
    static_assert(GreaterThanOrEqualComparable<int, double>,
                  "hyperion::platform::compare::GreaterThanOrEqualComparable test case 2 failing");
    static_assert(!GreaterThanOrEqualComparable<int, not_comparable>,
                  "hyperion::platform::compare::GreaterThanOrEqualComparable test case 3 failing");

#if HYPERION_PLATFORM_STD_LIB_HAS_COMPARE

    static_assert(ThreeWayComparable<int, int>,
                  "hyperion::platform::compare::ThreeWayComparable test case 1 failing");
    static_assert(ThreeWayComparable<int, double>,
                  "hyperion::platform::compare::ThreeWayComparable test case 2 failing");
    static_assert(!ThreeWayComparable<int, not_comparable>,
                  "hyperion::platform::compare::ThreeWayComparable test case 3 failing");

#endif // HYPERION_PLATFORM_STD_LIB_HAS_COMPARE

} // namespace hyperion::_test::platform::compare

#endif // HYPERION_PLATFORM_COMPARE_TESTS_H
