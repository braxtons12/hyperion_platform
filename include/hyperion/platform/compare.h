/// @file compare.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Comparison functions that are safe regardless of the being-compared types,
/// including the floating point types
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

#ifndef HYPERION_PLATFORM_COMPARE_H
#define HYPERION_PLATFORM_COMPARE_H

#include <hyperion/platform/def.h>
#include <hyperion/platform/ignore.h>
#include <hyperion/platform/types.h>

#include <cmath>
#include <concepts>
#include <cstdlib>
#include <limits>
#include <type_traits>

#if HYPERION_PLATFORM_STD_LIB_HAS_COMPARE
    #include <compare>
#endif // HYPERION_PLATFORM_STD_LIB_HAS_COMPARE

/// @ingroup utility
/// @{
///	@defgroup comparison Safe Comparison Functions
/// Hyperion provides safe comparison functions for safely comparing any types
/// providing comparison operators, including floating point types, as well as
/// mixed comparisons between signed, unsigned, and floating point types.
///
/// # Example
/// @code {.cpp}
/// using hyperion::operator""_f32;
/// const auto my_f32 = 12'345.6789_f32;
/// const auto my_other_f32 = 11'345.6789_f32;
/// const auto is_equal = hyperion::equality_compare(my_f32, my_other_f32);
/// @endcode
/// @headerfile hyperion/platform/compare.h
/// @}

namespace hyperion::platform::compare {

    /// @brief Concept definition requiring that a `TLhs` is
    /// equality comparable with a `TRhs`
    /// @ingroup comparison
    /// @headerfile hyperion/platform/compare.h
    template<typename TLhs, typename TRhs>
    concept EqualityComparable = requires(const TLhs& lhs, const TRhs& rhs) {
        lhs == rhs;
        rhs == lhs;
    };

    /// @brief Concept definition requiring that a `TLhs` is
    /// inequality comparable with a `TRhs`
    /// @ingroup comparison
    /// @headerfile hyperion/platform/compare.h
    template<typename TLhs, typename TRhs>
    concept InequalityComparable = requires(const TLhs& lhs, const TRhs& rhs) {
        lhs != rhs;
        rhs != lhs;
    };

    /// @brief Concept definition requiring that a `TLhs` is
    /// less-than comparable with a `TRhs`
    /// @ingroup comparison
    /// @headerfile hyperion/platform/compare.h
    template<typename TLhs, typename TRhs>
    concept LessThanComparable = requires(const TLhs& lhs, const TRhs& rhs) { lhs < rhs; };

    /// @brief Concept definition requiring that a `TLhs` is
    /// less-than-or-equal comparable with a `TRhs`
    /// @ingroup comparison
    /// @headerfile hyperion/platform/compare.h
    template<typename TLhs, typename TRhs>
    concept LessThanOrEqualComparable = requires(const TLhs& lhs, const TRhs& rhs) { lhs <= rhs; };

    /// @brief Concept definition requiring that a `TLhs` is
    /// greater-than comparable with a `TRhs`
    /// @ingroup comparison
    /// @headerfile hyperion/platform/compare.h
    template<typename TLhs, typename TRhs>
    concept GreaterThanComparable = requires(const TLhs& lhs, const TRhs& rhs) { lhs > rhs; };

    /// @brief Concept definition requiring that a `TLhs` is
    /// greater-than-or-equal comparable with a `TRhs`
    /// @ingroup comparison
    /// @headerfile hyperion/platform/compare.h
    template<typename TLhs, typename TRhs>
    concept GreaterThanOrEqualComparable
        = requires(const TLhs& lhs, const TRhs& rhs) { lhs >= rhs; };

#if HYPERION_PLATFORM_STD_LIB_HAS_COMPARE

    /// @brief Concept definition requiring that a `TLhs` is
    /// three-way comparable with a `TRhs`
    /// @ingroup comparison
    /// @headerfile hyperion/platform/compare.h
    template<typename TLhs, typename TRhs>
    concept ThreeWayComparable = std::three_way_comparable_with<TLhs, TRhs>;

#endif // HYPERION_PLATFORM_STD_LIB_HAS_COMPARE

    /// @brief Concept definition requiring that a type is
    /// truly arithmetic (a non-character, non-boolean integer type or floating point type)
    /// @ingroup comparison
    /// @headerfile hyperion/platform/compare.h
    template<typename TType>
    concept Arithmetic = std::is_arithmetic_v<std::remove_cvref_t<TType>>
                         && !(std::same_as<std::remove_cvref_t<TType>, bool>
                              || std::same_as<std::remove_cvref_t<TType>, char>
                              || std::same_as<std::remove_cvref_t<TType>, char8_t>
                              || std::same_as<std::remove_cvref_t<TType>, char16_t>
                              || std::same_as<std::remove_cvref_t<TType>, char32_t>);

    /// @brief Types of epsilons usable in floating point comparisons,
    /// either `Absolute`, i.e. a fixed magnitude difference, or `Relative`,
    /// i.e. a percentage magnitude difference.
    ///
    /// # Example
    /// @code{.cpp}
    /// static constexpr auto my_epsilon = make_epsilon<EpsilonType::Relative>{0.1_f64};
    /// @endcode
    ///
    /// @ingroup comparison
    /// @headerfile hyperion/platform/compare.h
    enum class EpsilonType : bool {
        Absolute,
        Relative,
    };

    namespace detail {
        template<Arithmetic TNumeric>
        constexpr auto abs(TNumeric value) noexcept -> TNumeric {
            return std::signbit(value) ? -value : value;
        }

        template<Arithmetic TLhs, Arithmetic TRhs>
        constexpr auto max(TLhs lhs, TRhs rhs) noexcept -> std::common_type_t<TLhs, TRhs> {
            return lhs < rhs ? rhs : lhs;
        }

        template<typename TLhs, typename TRhs>
        struct common_type {
            using type = std::nullptr_t;
        };

        template<typename TLhs, typename TRhs>
            requires(Arithmetic<TLhs>
                     && std::convertible_to<std::remove_cvref_t<TRhs>, std::remove_cvref_t<TLhs>>)
                    || (Arithmetic<TRhs>
                        && std::convertible_to<std::remove_cvref_t<TLhs>,
                                               std::remove_cvref_t<TRhs>>)
        struct common_type<TLhs, TRhs> : public std::common_type<TLhs, TRhs> { };

        template<typename TLhs, typename TRhs>
        using common_type_t = typename common_type<TLhs, TRhs>::type;

        template<EpsilonType TType, typename TLhs, typename TRhs>
        static inline constexpr auto default_epsilon = []() {
            using common_t = common_type_t<TLhs, TRhs>;
            if constexpr(TType == EpsilonType::Absolute
                         && not std::same_as<common_t, std::nullptr_t>)
            {
                return std::numeric_limits<common_t>::epsilon();
            }
            else {
                return static_cast<
                    std::conditional_t<std::same_as<common_t, std::nullptr_t>, fmax, common_t>>(
                    0.001_fmax);
            }
        }();

        constexpr auto safe_float_equality(std::floating_point auto lhs,
                                           std::floating_point auto rhs,
                                           std::floating_point auto error) noexcept -> bool {
            using common_t
                = common_type_t<common_type_t<decltype(lhs), decltype(rhs)>, decltype(error)>;

            const auto left = static_cast<common_t>(lhs);
            const auto right = static_cast<common_t>(rhs);
            const auto err = static_cast<common_t>(error);

            const auto diff = left - right;

            if(std::isinf(diff) || std::isnan(diff)) {
                const auto lower_bound = right - err;
                const auto upper_bound = right + err;
                return left >= lower_bound && left <= upper_bound;
            }

            return detail::abs(diff) <= err;
        }

        constexpr auto safe_float_inequality(std::floating_point auto lhs,
                                             std::floating_point auto rhs,
                                             std::floating_point auto error) noexcept -> bool {
            using common_t
                = common_type_t<common_type_t<decltype(lhs), decltype(rhs)>, decltype(error)>;

            const auto left = static_cast<common_t>(lhs);
            const auto right = static_cast<common_t>(rhs);
            const auto err = static_cast<common_t>(error);

            const auto diff = left - right;

            if(std::isinf(diff) || std::isnan(diff)) {
                const auto lower_bound = right - err;
                const auto upper_bound = right + err;
                return left < lower_bound || left > upper_bound;
            }

            return detail::abs(diff) > err;
        }
    } // namespace detail

    /// @brief Represents an `Absolute` or `Relative` epsilon of a specific `Arithmetic` type.
    ///
    /// # Example
    /// @code{.cpp}
    /// static constexpr auto my_epsilon = make_epsilon<EpsilonType::Relative>{0.1_f64};
    /// @endcode
    ///
    /// @tparam TType The `EpsilonType` this `Epsilon` represents
    /// @tparam TNumeric The `Arithmetic` type of this `Epsilon`
    /// @ingroup comparison
    /// @headerfile hyperion/platform/compare.h
    template<EpsilonType TType = EpsilonType::Absolute, Arithmetic TNumeric = fmax>
    class Epsilon {
      public:
        constexpr Epsilon() noexcept = default;
        constexpr Epsilon(const Epsilon&) noexcept = default;
        constexpr Epsilon(Epsilon&&) noexcept = default;
        constexpr ~Epsilon() noexcept = default;
        constexpr auto operator=(const Epsilon&) noexcept -> Epsilon& = default;
        constexpr auto operator=(Epsilon&&) noexcept -> Epsilon& = default;

        /// @brief Constructs an `Epsilon` with the given numeric value
        ///
        /// @param val The numeric value of this `Epsilon`
        explicit(false) constexpr Epsilon(TNumeric val) // NOLINT(*-explicit-*)
            : m_epsilon(val) {
        }

        /// @brief Returns the `Absolute` epsilon this `Epsilon` would represent when used
        /// in a comparison between the two arguments
        ///
        /// @param lhs The left-hand argument of the theoretical comparsion
        /// @param rhs The right-hand argument of the theoretical comparsion
        /// @return The `Absolute` epsilon this `Epsilon` represents when used in a comparision
        /// between `lhs` and `rhs`
        constexpr auto
        epsilon(const Arithmetic auto& lhs, const Arithmetic auto& rhs) const noexcept -> TNumeric {
            ignore(lhs, rhs);
            if constexpr(TType == EpsilonType::Absolute) {
                return m_epsilon;
            }
            else {
                using result_t = decltype(detail::max(detail::abs(lhs), detail::abs(rhs)));
                using common_type = std::common_type_t<TNumeric, result_t>;
                return static_cast<TNumeric>(
                    static_cast<common_type>(m_epsilon)
                    * static_cast<common_type>(detail::max(detail::abs(lhs), detail::abs(rhs))));
            }
        }

        /// @brief Returns the numeric value of this `Epsilon`
        /// @return The value of the `Epsilon`
        constexpr auto value() const noexcept -> TNumeric {
            return m_epsilon;
        }

      private:
        TNumeric m_epsilon = detail::default_epsilon<TType, TNumeric, TNumeric>;
    };

    namespace detail {
        template<typename TLhs, typename TRhs, EpsilonType TType = EpsilonType::Absolute>
        constexpr auto make_epsilon() noexcept -> Epsilon<
            TType,
            std::conditional_t<std::same_as<detail::common_type_t<TLhs, TRhs>, std::nullptr_t>,
                               fmax,
                               detail::common_type_t<TLhs, TRhs>>> {
            return {detail::default_epsilon<TType, TLhs, TRhs>};
        }
    } // namespace detail

    /// @brief Returns an `Epsilon` of the specified `EpsilonType` and given value.
    ///
    /// # Example
    /// @code{.cpp}
    /// static constexpr auto my_epsilon = make_epsilon<EpsilonType::Relative>{0.1_f64};
    /// @endcode
    ///
    /// @tparam TType The `EpsilonType` of the epsilon
    /// @tparam TEpsilon The arithmetic type of the epsilon (e.g., `f64`, or `int`)
    /// @param epsilon the arithmetic value of the epsilon (e.g. `0.001_f64`)
    /// @return The `Epsilon`
    /// @ingroup comparison
    /// @headerfile hyperion/platform/compare.h
    template<EpsilonType TType, Arithmetic TEpsilon>
    constexpr auto make_epsilon(TEpsilon&& epsilon) -> Epsilon<TType, TEpsilon> {
        return {std::forward<TEpsilon>(epsilon)};
    }

    /// @brief Type trait to check whether `TType` is a specialization of `Epsilon`
    /// @tparam TType The type to check
    /// @ingroup comparison
    /// @headerfile hyperion/platform/compare.h
    template<typename TType>
    struct is_epsilon_specialization : std::false_type { };

    template<EpsilonType TType, Arithmetic TNumeric>
    struct is_epsilon_specialization<Epsilon<TType, TNumeric>> : std::true_type { };

    /// @brief Value of the type trait `is_epsilon_specialization`.
    /// Used to check whether `TType` is a specialization of `Epsilon`
    /// @tparam TType The type to check
    /// @ingroup comparison
    /// @headerfile hyperion/platform/compare.h
    template<typename TType>
    static inline constexpr auto is_epsilon_specialization_v
        = is_epsilon_specialization<TType>::value;

    /// @brief Concept requiring that `TType` is a specialization of `Epsilon`
    /// @ingroup comparison
    /// @headerfile hyperion/platform/compare.h
    template<typename TType>
    concept EpsilonKind = is_epsilon_specialization_v<std::remove_cvref_t<TType>>;

    HYPERION_IGNORE_FLOAT_EQUALITY_WARNING_START;

#if HYPERION_PLATFORM_COMPILER_IS_CLANG
    _Pragma("GCC diagnostic push");
    _Pragma("GCC diagnostic ignored \"-Wdouble-promotion\"");
    _Pragma("GCC diagnostic ignored \"-Wimplicit-int-float-conversion\"");
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

    /// @brief Safely compares `lhs` and `rhs` for equality, taking into account signedness
    /// differences and accounting for floating point inaccuracies with an `Epsilon`
    ///
    /// # Example
    /// @code{.cpp}
    /// constexpr auto my_epsilon = Epsilon<EpsilonType::Relative>{0.1_f64};
    ///
    /// auto value1 = getSomeValue(1);
    /// auto value2 = getSomeValue(2);
    /// // check that value1 and value2 are within 10% of whichever is largest
    /// auto is_equal = equality_compare(value1, value2, my_epsilon);
    /// @endcode
    ///
    /// @tparam TLhs The type of the left-hand argument in the comparison
    /// @tparam TRhs The type of the right-hand argument in the comparison
    /// Defaults to an `Absolute` epsilon of type `fmax`.
    /// @param lhs The left-hand argument in the comparison
    /// @param rhs The right-hand argument in the comparison
    /// @param epsilon The `Epsilon` used for floating point comparison.
    /// Defaults to an `Absolute` epsilon equal to the machine epsilon corresponding with
    /// the type that is the wider of the two types `TLhs` and `TRhs`.
    /// @return Whether `lhs` and `rhs` are equal
    /// @ingroup comparison
    /// @headerfile hyperion/platform/compare.h
    template<typename TLhs,
             typename TRhs,
             EpsilonKind TEpsilon = decltype(detail::make_epsilon<TLhs, TRhs>())>
        requires EqualityComparable<TLhs, TRhs>
    constexpr auto equality_compare(
        TLhs&& lhs,
        TRhs&& rhs,
        TEpsilon epsilon
        = detail::make_epsilon<TLhs, TRhs>()) noexcept(noexcept(lhs == rhs) && noexcept(rhs == lhs))
        -> bool {

#if HYPERION_PLATFORM_COMPILER_IS_CLANG
        _Pragma("GCC diagnostic pop");
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG
       //
        using lhs_t = std::remove_cvref_t<TLhs>;
        using rhs_t = std::remove_cvref_t<TRhs>;

        static_assert(
            std::numeric_limits<f32>::is_iec559
                || (!std::floating_point<lhs_t> && !std::floating_point<TRhs>),
            "Hyperion's safe comparisons require the platform to be one that uses IEEE754 "
            "floating point numbers");

        if constexpr(std::integral<lhs_t> && std::integral<rhs_t>) {
            if constexpr(std::is_signed_v<lhs_t> && !std::is_signed_v<rhs_t>) {
                if(std::signbit(lhs)) {
                    return false;
                }

                return static_cast<rhs_t>(lhs) == rhs;
            }
            else if constexpr(std::is_signed_v<rhs_t> && !std::is_signed_v<rhs_t>) {
                if(std::signbit(rhs)) {
                    return false;
                }

                return lhs == static_cast<lhs_t>(rhs);
            }
            else {
                return lhs == rhs;
            }
        }
        else if constexpr(std::floating_point<lhs_t> || std::floating_point<rhs_t>) {
            if constexpr(std::integral<lhs_t>) {
                return equality_compare(static_cast<fmax>(lhs),
                                        std::forward<TRhs>(rhs),
                                        std::move(epsilon));
            }
            else if constexpr(std::integral<rhs_t>) {
                return equality_compare(std::forward<TLhs>(lhs),
                                        static_cast<fmax>(rhs),
                                        std::move(epsilon));
            }
            else {
                if(std::isnan(lhs) || std::isinf(lhs) || std::isnan(rhs) || std::isinf(rhs)) {
                    return false;
                }

                const auto error = epsilon.epsilon(lhs, rhs);
                return detail::safe_float_equality(lhs, rhs, error);
            }
        }
        else {
            return std::forward<TLhs>(lhs) == std::forward<TRhs>(rhs);
        }
    }

#if HYPERION_PLATFORM_COMPILER_IS_CLANG
    _Pragma("GCC diagnostic push");
    _Pragma("GCC diagnostic ignored \"-Wdouble-promotion\"");
    _Pragma("GCC diagnostic ignored \"-Wimplicit-int-float-conversion\"");
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

    /// @brief Safely compares `lhs` and `rhs` for inequality, taking into account signedness
    /// differences and accounting for floating point inaccuracies with an `Epsilon`
    ///
    /// # Example
    /// @code{.cpp}
    /// constexpr auto my_epsilon = Epsilon<EpsilonType::Relative>{0.1_f64};
    ///
    /// auto value1 = getSomeValue(1);
    /// auto value2 = getSomeValue(2);
    /// // check that value1 and value2 are _not_ within 10% of whichever is largest
    /// auto is_equal = inequality_compare(value1, value2, my_epsilon);
    /// @endcode
    ///
    /// @tparam TLhs The type of the left-hand argument in the comparison
    /// @tparam TRhs The type of the right-hand argument in the comparison
    /// Defaults to an `Absolute` epsilon of type `fmax`.
    /// @param lhs The left-hand argument in the comparison
    /// @param rhs The right-hand argument in the comparison
    /// @param epsilon The `Epsilon` used for floating point comparison.
    /// Defaults to an `Absolute` epsilon equal to the machine epsilon corresponding with
    /// the type that is the wider of the two types `TLhs` and `TRhs`.
    /// @return Whether `lhs` and `rhs` are _not_ equal
    /// @ingroup comparison
    /// @headerfile hyperion/platform/compare.h
    template<typename TLhs,
             typename TRhs,
             EpsilonKind TEpsilon = decltype(detail::make_epsilon<TLhs, TRhs>())>
        requires InequalityComparable<TLhs, TRhs>
    constexpr auto inequality_compare(
        TLhs&& lhs,
        TRhs&& rhs,
        TEpsilon epsilon
        = detail::make_epsilon<TLhs, TRhs>()) noexcept(noexcept(lhs == rhs) && noexcept(rhs == lhs))
        -> bool {

#if HYPERION_PLATFORM_COMPILER_IS_CLANG
        _Pragma("GCC diagnostic pop");
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

        using lhs_t = std::remove_cvref_t<TLhs>;
        using rhs_t = std::remove_cvref_t<TRhs>;

        static_assert(
            std::numeric_limits<f32>::is_iec559
                || (!std::floating_point<lhs_t> && !std::floating_point<TRhs>),
            "Hyperion's safe comparisons require the platform to be one that uses IEEE754 "
            "floating point numbers");

        if constexpr(std::integral<lhs_t> && std::integral<rhs_t>) {
            if constexpr(std::is_signed_v<lhs_t> && !std::is_signed_v<rhs_t>) {
                if(std::signbit(lhs)) {
                    return true;
                }

                return static_cast<rhs_t>(lhs) != rhs;
            }
            else if constexpr(std::is_signed_v<rhs_t> && !std::is_signed_v<rhs_t>) {
                if(std::signbit(rhs)) {
                    return true;
                }

                return lhs != static_cast<lhs_t>(rhs);
            }
            else {
                return lhs != rhs;
            }
        }
        else if constexpr(std::floating_point<lhs_t> || std::floating_point<rhs_t>) {
            if constexpr(std::integral<lhs_t>) {
                return inequality_compare(static_cast<fmax>(lhs),
                                          std::forward<TRhs>(rhs),
                                          std::move(epsilon));
            }
            else if constexpr(std::integral<rhs_t>) {
                return inequality_compare(std::forward<TLhs>(lhs),
                                          static_cast<fmax>(rhs),
                                          std::move(epsilon));
            }
            else {
                if(std::isnan(lhs) || std::isinf(lhs) || std::isnan(rhs) || std::isinf(rhs)) {
                    return true;
                }

                const auto error = epsilon.epsilon(lhs, rhs);
                return detail::safe_float_inequality(lhs, rhs, error);
            }
        }
        else {
            return std::forward<TLhs>(lhs) != std::forward<TRhs>(rhs);
        }
    }

#if HYPERION_PLATFORM_COMPILER_IS_CLANG
    _Pragma("GCC diagnostic push");
    _Pragma("GCC diagnostic ignored \"-Wdouble-promotion\"");
    _Pragma("GCC diagnostic ignored \"-Wimplicit-int-float-conversion\"");
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

    /// @brief Safely compares whether `lhs` is less than `rhs`, taking into account signedness
    /// differences and accounting for floating point inaccuracies with an `Epsilon`
    ///
    /// # Example
    /// @code{.cpp}
    /// constexpr auto my_epsilon = Epsilon<EpsilonType::Relative>{0.1_f64};
    ///
    /// auto value1 = getSomeValue(1);
    /// auto value2 = getSomeValue(2);
    /// // check that value1 is less than value2 by at least 10%
    /// auto is_equal = less_than_compare(value1, value2, my_epsilon);
    /// @endcode
    ///
    /// @tparam TLhs The type of the left-hand argument in the comparison
    /// @tparam TRhs The type of the right-hand argument in the comparison
    /// Defaults to an `Absolute` epsilon of type `fmax`.
    /// @param lhs The left-hand argument in the comparison
    /// @param rhs The right-hand argument in the comparison
    /// @param epsilon The `Epsilon` used for floating point comparison.
    /// Defaults to an `Absolute` epsilon equal to the machine epsilon corresponding with
    /// the type that is the wider of the two types `TLhs` and `TRhs`.
    /// @return Whether `lhs` is less than `rhs`
    /// @ingroup comparison
    /// @headerfile hyperion/platform/compare.h
    template<typename TLhs,
             typename TRhs,
             EpsilonKind TEpsilon = decltype(detail::make_epsilon<TLhs, TRhs>())>
        requires LessThanComparable<TLhs, TRhs>
    // NOLINTNEXTLINE(*-cognitive-complexity)
    constexpr auto less_than_compare(
        TLhs&& lhs,
        TRhs&& rhs,
        TEpsilon epsilon
        = detail::make_epsilon<TLhs, TRhs>()) noexcept(noexcept(lhs == rhs) && noexcept(rhs == lhs))
        -> bool {

#if HYPERION_PLATFORM_COMPILER_IS_CLANG
        _Pragma("GCC diagnostic pop");
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

        using lhs_t = std::remove_cvref_t<TLhs>;
        using rhs_t = std::remove_cvref_t<TRhs>;

        static_assert(
            std::numeric_limits<f32>::is_iec559
                || (!std::floating_point<lhs_t> && !std::floating_point<TRhs>),
            "Hyperion's safe comparisons require the platform to be one that uses IEEE754 "
            "floating point numbers");

        if constexpr(std::integral<lhs_t> && std::integral<rhs_t>) {
            if constexpr(std::is_signed_v<lhs_t> && !std::is_signed_v<rhs_t>) {
                if(std::signbit(lhs)) {
                    return true;
                }

                return static_cast<rhs_t>(lhs) < rhs;
            }
            else if constexpr(std::is_signed_v<rhs_t> && !std::is_signed_v<lhs_t>) {
                if(std::signbit(rhs)) {
                    return false;
                }

                return lhs < static_cast<lhs_t>(rhs);
            }
            else {
                return lhs < rhs;
            }
        }
        else if constexpr(std::floating_point<lhs_t> || std::floating_point<rhs_t>) {
            if constexpr(std::integral<lhs_t>) {
                return less_than_compare(static_cast<fmax>(lhs),
                                         std::forward<TRhs>(rhs),
                                         std::move(epsilon));
            }
            else if constexpr(std::integral<rhs_t>) {
                return less_than_compare(std::forward<TLhs>(lhs),
                                         static_cast<fmax>(rhs),
                                         std::move(epsilon));
            }
            else {
                if(std::isinf(lhs) && std::signbit(lhs)) {
                    return true;
                }

                if(std::isinf(rhs) && std::signbit(rhs)) {
                    return false;
                }

                if(std::isnan(lhs) || std::isnan(rhs)) {
                    return false;
                }

                const auto error = epsilon.epsilon(lhs, rhs);

                using common_type = detail::common_type_t<
                    detail::common_type_t<std::remove_cvref_t<TLhs>, std::remove_cvref_t<TRhs>>,
                    std::remove_cvref_t<decltype(error)>>;

                return static_cast<common_type>(lhs)
                       < static_cast<common_type>(rhs) - static_cast<common_type>(error);
            }
        }
        else {
            return std::forward<TLhs>(lhs) < std::forward<TRhs>(rhs);
        }
    }

#if HYPERION_PLATFORM_COMPILER_IS_CLANG
    _Pragma("GCC diagnostic push");
    _Pragma("GCC diagnostic ignored \"-Wdouble-promotion\"");
    _Pragma("GCC diagnostic ignored \"-Wimplicit-int-float-conversion\"");
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

    /// @brief Safely compares whether `lhs` is less than or equal to `rhs`,
    /// taking into account signedness differences and accounting for floating point
    /// inaccuracies with an `Epsilon`
    ///
    /// # Example
    /// @code{.cpp}
    /// constexpr auto my_epsilon = Epsilon<EpsilonType::Relative>{0.1_f64};
    ///
    /// auto value1 = getSomeValue(1);
    /// auto value2 = getSomeValue(2);
    /// // check that value1 is less than value2 by at least 10%,
    /// // or that value1 and value2 are within 10% of whichever is largest
    /// auto is_equal = less_than_or_equal_compare(value1, value2, my_epsilon);
    /// @endcode
    ///
    /// @tparam TLhs The type of the left-hand argument in the comparison
    /// @tparam TRhs The type of the right-hand argument in the comparison
    /// Defaults to an `Absolute` epsilon of type `fmax`.
    /// @param lhs The left-hand argument in the comparison
    /// @param rhs The right-hand argument in the comparison
    /// @param epsilon The `Epsilon` used for floating point comparison.
    /// Defaults to an `Absolute` epsilon equal to the machine epsilon corresponding with
    /// the type that is the wider of the two types `TLhs` and `TRhs`.
    /// @return Whether `lhs` is less than or equal to `rhs`
    /// @ingroup comparison
    /// @headerfile hyperion/platform/compare.h
    template<typename TLhs,
             typename TRhs,
             EpsilonKind TEpsilon = decltype(detail::make_epsilon<TLhs, TRhs>())>
        requires LessThanOrEqualComparable<TLhs, TRhs>
    // NOLINTNEXTLINE(*-cognitive-complexity)
    constexpr auto less_than_or_equal_compare(
        TLhs&& lhs,
        TRhs&& rhs,
        TEpsilon epsilon
        = detail::make_epsilon<TLhs, TRhs>()) noexcept(noexcept(lhs == rhs) && noexcept(rhs == lhs))
        -> bool {

#if HYPERION_PLATFORM_COMPILER_IS_CLANG
        _Pragma("GCC diagnostic pop");
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

        using lhs_t = std::remove_cvref_t<TLhs>;
        using rhs_t = std::remove_cvref_t<TRhs>;

        static_assert(
            std::numeric_limits<f32>::is_iec559
                || (!std::floating_point<lhs_t> && !std::floating_point<TRhs>),
            "Hyperion's safe comparisons require the platform to be one that uses IEEE754 "
            "floating point numbers");

        if constexpr(std::integral<lhs_t> && std::integral<rhs_t>) {
            if constexpr(std::is_signed_v<lhs_t> && !std::is_signed_v<rhs_t>) {
                if(std::signbit(lhs)) {
                    return true;
                }

                return static_cast<rhs_t>(lhs) <= rhs;
            }
            else if constexpr(std::is_signed_v<rhs_t> && !std::is_signed_v<lhs_t>) {
                if(std::signbit(rhs)) {
                    return false;
                }

                return lhs <= static_cast<lhs_t>(rhs);
            }
            else {
                return lhs <= rhs;
            }
        }
        else if constexpr(std::floating_point<lhs_t> || std::floating_point<rhs_t>) {
            if constexpr(std::integral<lhs_t>) {
                return less_than_or_equal_compare(static_cast<fmax>(lhs),
                                                  std::forward<TRhs>(rhs),
                                                  std::move(epsilon));
            }
            else if constexpr(std::integral<rhs_t>) {
                return less_than_or_equal_compare(std::forward<TLhs>(lhs),
                                                  static_cast<fmax>(rhs),
                                                  std::move(epsilon));
            }
            else {
                if(std::isinf(lhs) && std::signbit(lhs)) {
                    return true;
                }

                if(std::isinf(rhs) && std::signbit(rhs)) {
                    return false;
                }

                if(std::isnan(lhs) || std::isnan(rhs)) {
                    return false;
                }

                const auto error = epsilon.epsilon(lhs, rhs);
                using common_type = detail::common_type_t<
                    detail::common_type_t<std::remove_cvref_t<TLhs>, std::remove_cvref_t<TRhs>>,
                    std::remove_cvref_t<decltype(error)>>;

                const auto less = static_cast<common_type>(lhs)
                                  < static_cast<common_type>(rhs) - static_cast<common_type>(error);
                return less || detail::safe_float_equality(lhs, rhs, error);
            }
        }
        else {
            return std::forward<TLhs>(lhs) <= std::forward<TRhs>(rhs);
        }
    }

#if HYPERION_PLATFORM_COMPILER_IS_CLANG
    _Pragma("GCC diagnostic push");
    _Pragma("GCC diagnostic ignored \"-Wdouble-promotion\"");
    _Pragma("GCC diagnostic ignored \"-Wimplicit-int-float-conversion\"");
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

    /// @brief Safely compares whether `lhs` is greater than `rhs`, taking into account
    /// signedness differences and accounting for floating point inaccuracies with an `Epsilon`
    ///
    /// # Example
    /// @code{.cpp}
    /// constexpr auto my_epsilon = Epsilon<EpsilonType::Relative>{0.1_f64};
    ///
    /// auto value1 = getSomeValue(1);
    /// auto value2 = getSomeValue(2);
    /// // check that value1 is greater than value2 by at least 10%
    /// auto is_equal = greater_than_compare(value1, value2, my_epsilon);
    /// @endcode
    ///
    /// @tparam TLhs The type of the left-hand argument in the comparison
    /// @tparam TRhs The type of the right-hand argument in the comparison
    /// Defaults to an `Absolute` epsilon of type `fmax`.
    /// @param lhs The left-hand argument in the comparison
    /// @param rhs The right-hand argument in the comparison
    /// @param epsilon The `Epsilon` used for floating point comparison.
    /// Defaults to an `Absolute` epsilon equal to the machine epsilon corresponding with
    /// the type that is the wider of the two types `TLhs` and `TRhs`.
    /// @return Whether `lhs` is greater than `rhs`
    /// @ingroup comparison
    /// @headerfile hyperion/platform/compare.h
    template<typename TLhs,
             typename TRhs,
             EpsilonKind TEpsilon = decltype(detail::make_epsilon<TLhs, TRhs>())>
        requires LessThanComparable<TLhs, TRhs>
    // NOLINTNEXTLINE(*-cognitive-complexity)
    constexpr auto greater_than_compare(
        TLhs&& lhs,
        TRhs&& rhs,
        TEpsilon epsilon
        = detail::make_epsilon<TLhs, TRhs>()) noexcept(noexcept(lhs == rhs) && noexcept(rhs == lhs))
        -> bool {

#if HYPERION_PLATFORM_COMPILER_IS_CLANG
        _Pragma("GCC diagnostic pop");
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

        using lhs_t = std::remove_cvref_t<TLhs>;
        using rhs_t = std::remove_cvref_t<TRhs>;

        static_assert(
            std::numeric_limits<f32>::is_iec559
                || (!std::floating_point<lhs_t> && !std::floating_point<TRhs>),
            "Hyperion's safe comparisons require the platform to be one that uses IEEE754 "
            "floating point numbers");

        if constexpr(std::integral<lhs_t> && std::integral<rhs_t>) {
            if constexpr(std::is_signed_v<lhs_t> && !std::is_signed_v<rhs_t>) {
                if(std::signbit(lhs)) {
                    return false;
                }

                return static_cast<rhs_t>(lhs) > rhs;
            }
            else if constexpr(std::is_signed_v<rhs_t> && !std::is_signed_v<lhs_t>) {
                if(std::signbit(rhs)) {
                    return true;
                }

                return lhs > static_cast<lhs_t>(rhs);
            }
            else {
                return lhs > rhs;
            }
        }
        else if constexpr(std::floating_point<lhs_t> || std::floating_point<rhs_t>) {
            if constexpr(std::integral<lhs_t>) {
                return greater_than_compare(static_cast<fmax>(lhs),
                                            std::forward<TRhs>(rhs),
                                            std::move(epsilon));
            }
            else if constexpr(std::integral<rhs_t>) {
                return greater_than_compare(std::forward<TLhs>(lhs),
                                            static_cast<fmax>(rhs),
                                            std::move(epsilon));
            }
            else {
                if(std::isinf(lhs) && std::signbit(lhs)) {
                    return false;
                }

                if(std::isinf(rhs) && std::signbit(rhs)) {
                    return true;
                }

                if(std::isnan(lhs) || std::isnan(rhs)) {
                    return false;
                }

                const auto error = epsilon.epsilon(lhs, rhs);
                using common_type = detail::common_type_t<
                    detail::common_type_t<std::remove_cvref_t<TLhs>, std::remove_cvref_t<TRhs>>,
                    std::remove_cvref_t<decltype(error)>>;

                return static_cast<common_type>(lhs) - static_cast<common_type>(error)
                       > static_cast<common_type>(rhs);
            }
        }
        else {
            return std::forward<TLhs>(lhs) > std::forward<TRhs>(rhs);
        }
    }

#if HYPERION_PLATFORM_COMPILER_IS_CLANG
    _Pragma("GCC diagnostic push");
    _Pragma("GCC diagnostic ignored \"-Wdouble-promotion\"");
    _Pragma("GCC diagnostic ignored \"-Wimplicit-int-float-conversion\"");
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

    /// @brief Safely compares whether `lhs` is greater than or equal to `rhs`,
    /// taking into account signedness differences and accounting for floating point
    /// inaccuracies with an `Epsilon`
    ///
    /// # Example
    /// @code{.cpp}
    /// constexpr auto my_epsilon = Epsilon<EpsilonType::Relative>{0.1_f64};
    ///
    /// auto value1 = getSomeValue(1);
    /// auto value2 = getSomeValue(2);
    /// // check that value1 is greater than value2 by at least 10%,
    /// // or that value1 and value2 are within 10% of whichever is largest
    /// auto is_equal = less_than_or_equal_compare(value1, value2, my_epsilon);
    /// @endcode
    ///
    /// @tparam TLhs The type of the left-hand argument in the comparison
    /// @tparam TRhs The type of the right-hand argument in the comparison
    /// Defaults to an `Absolute` epsilon of type `fmax`.
    /// @param lhs The left-hand argument in the comparison
    /// @param rhs The right-hand argument in the comparison
    /// @param epsilon The `Epsilon` used for floating point comparison.
    /// Defaults to an `Absolute` epsilon equal to the machine epsilon corresponding with
    /// the type that is the wider of the two types `TLhs` and `TRhs`.
    /// @return Whether `lhs` is greater than or equal to `rhs`
    /// @ingroup comparison
    /// @headerfile hyperion/platform/compare.h
    template<typename TLhs,
             typename TRhs,
             EpsilonKind TEpsilon = decltype(detail::make_epsilon<TLhs, TRhs>())>
        requires LessThanOrEqualComparable<TLhs, TRhs>
    // NOLINTNEXTLINE(*-cognitive-complexity)
    constexpr auto greater_than_or_equal_compare(
        TLhs&& lhs,
        TRhs&& rhs,
        TEpsilon epsilon
        = detail::make_epsilon<TLhs, TRhs>()) noexcept(noexcept(lhs == rhs) && noexcept(rhs == lhs))
        -> bool {

#if HYPERION_PLATFORM_COMPILER_IS_CLANG
        _Pragma("GCC diagnostic pop");
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

        using lhs_t = std::remove_cvref_t<TLhs>;
        using rhs_t = std::remove_cvref_t<TRhs>;

        static_assert(
            std::numeric_limits<f32>::is_iec559
                || (!std::floating_point<lhs_t> && !std::floating_point<TRhs>),
            "Hyperion's safe comparisons require the platform to be one that uses IEEE754 "
            "floating point numbers");

        if constexpr(std::integral<lhs_t> && std::integral<rhs_t>) {
            if constexpr(std::is_signed_v<lhs_t> && !std::is_signed_v<rhs_t>) {
                if(std::signbit(lhs)) {
                    return false;
                }

                return static_cast<rhs_t>(lhs) >= rhs;
            }
            else if constexpr(std::is_signed_v<rhs_t> && !std::is_signed_v<lhs_t>) {
                if(std::signbit(rhs)) {
                    return true;
                }

                return lhs >= static_cast<lhs_t>(rhs);
            }
            else {
                return lhs >= rhs;
            }
        }
        else if constexpr(std::floating_point<lhs_t> || std::floating_point<rhs_t>) {
            if constexpr(std::integral<lhs_t>) {
                return greater_than_or_equal_compare(static_cast<fmax>(lhs),
                                                     std::forward<TRhs>(rhs),
                                                     std::move(epsilon));
            }
            else if constexpr(std::integral<rhs_t>) {
                return greater_than_or_equal_compare(std::forward<TLhs>(lhs),
                                                     static_cast<fmax>(rhs),
                                                     std::move(epsilon));
            }
            else {
                if(std::isinf(lhs) && std::signbit(lhs)) {
                    return false;
                }

                if(std::isinf(rhs) && std::signbit(rhs)) {
                    return true;
                }

                if(std::isnan(lhs) || std::isnan(rhs)) {
                    return false;
                }

                const auto error = epsilon.epsilon(lhs, rhs);
                using common_type = detail::common_type_t<
                    detail::common_type_t<std::remove_cvref_t<TLhs>, std::remove_cvref_t<TRhs>>,
                    std::remove_cvref_t<decltype(error)>>;

                const auto greater = static_cast<common_type>(lhs) - static_cast<common_type>(error)
                                     > static_cast<common_type>(rhs);
                return greater || detail::safe_float_equality(lhs, rhs, error);
            }
        }
        else {
            return std::forward<TLhs>(lhs) >= std::forward<TRhs>(rhs);
        }
    }

    HYPERION_IGNORE_FLOAT_EQUALITY_WARNING_STOP;
} // namespace hyperion::platform::compare

#endif // HYPERION_PLATFORM_COMPARE_H
