/// @file types.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief various type aliases for builtin types and user defined literals for them
/// @version 0.5.3
/// @date 2024-09-25
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

#ifndef HYPERION_PLATFORM_TYPES_H
#define HYPERION_PLATFORM_TYPES_H

#include <hyperion/platform/def.h>

#include <fast_float/fast_float.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>

/// @ingroup platform
/// @{
///	@defgroup core_numeric_types Core Numeric Types
/// Hyperion provides type aliases for fixed-width integer and floating point types,
/// as well as the platform native size-type, byte, and maximum-width integer and floating point
/// types. It also provides user-defined literals for each that support numeric separators
/// (eg "12'345") and provide bounds checking to ensure a literal is not out of the range of its
/// associated type.
///
/// # Example
/// @code {.cpp}
/// using hyperion::operator""_f32;
/// const auto my_f32 = 12'345.6789_f32;
/// @endcode
/// @headerfile hyperion/platform/types.h
/// @}

namespace hyperion {

    /// @brief `u8` is an 8-bit unsigned integer
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    using u8 = uint8_t;
    /// @brief `u16` is an 16-bit unsigned integer
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    using u16 = uint16_t;
    /// @brief `u32` is an 32-bit unsigned integer
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    using u32 = uint32_t;
    /// @brief `u64` is an 64-bit unsigned integer
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    using u64 = uint64_t;
    /// @brief `usize` is the unsigned integer type best suited for sizes on the given platform
    /// (`size_t`)
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    using usize = size_t;
    /// @brief `umax` is the largest unsigned integer on the platform (`uintmax_t`)
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    using umax = uintmax_t;

    /// @brief `i8` is an 8-bit signed integer
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    using i8 = int8_t;
    /// @brief `i16` is a 16-bit signed integer
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    using i16 = int16_t;
    /// @brief `i32` is a 32-bit signed integer
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    using i32 = int32_t;
    /// @brief `i64` is a 64-bit signed integer
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    using i64 = int64_t;
    /// @brief `imax` is the largest signed integer on the platform (`intmax_t`)
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    using imax = intmax_t;

    /// @brief `byte` is a single-byte unsigned integer
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    using byte = unsigned char;

    /// @brief `f32` is a single-precision (32-bit) floating point number
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    using f32 = float;
    /// @brief `f64` is a double-precision (64-bit) floating point number
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    using f64 = double;
    /// @brief `fmax` is the largest-precision floating point number on the platform (`long double`)
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    using fmax = long double;

    namespace detail {
        template<char... Chars>
        struct string_literal {
            std::array<char, sizeof...(Chars)> array = {Chars...};
        };

        template<char... Chars>
        constexpr auto trim_separators([[maybe_unused]] string_literal<Chars...> literal) noexcept {
            constexpr auto new_size = []() {
                constexpr auto array = std::array{Chars...};
                auto size = array.size();
                for(const auto& c : array) {
                    if(c == '\'') {
                        size--;
                    }
                }
                return size;
            }();

            constexpr auto array = std::array{Chars...};
            std::array<char, new_size> new_array = {};
            auto i = 0UL;
            for(const auto& c : array) {
                if(c != '\'') {
                    new_array[i] = c;
                    i++;
                }
            }

            return new_array;
        }

        enum class literal_status : u8 {
            Valid = 0,
            OutOfRange,
            InvalidCharacterSequence,
            InvalidLiteralType,
        };

        template<literal_status status>
        static constexpr auto check_literal_status() noexcept -> void {
            static_assert(status != detail::literal_status::OutOfRange,
                          "Invalid Literal: Literal out of numeric range for type");
            static_assert(status != detail::literal_status::InvalidCharacterSequence,
                          "Invalid Literal: Literal contains invalid character sequence for type");
            static_assert(status != detail::literal_status::InvalidLiteralType,
                          "Invalid Literal: Requested type is not a valid numeric literal type");
        }

        HYPERION_IGNORE_PADDING_WARNING_START
        template<typename T>
        struct literal_pair {
            literal_status status = literal_status::Valid;
            T value = T(0);
        };
        HYPERION_IGNORE_PADDING_WARNING_START

        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START
        template<typename Type, char... Chars, usize N = sizeof...(Chars)>
        [[nodiscard]] static consteval auto
        // NOLINTNEXTLINE(readability-function-cognitive-complexity)
        parse_literal(string_literal<Chars...> literal) noexcept -> literal_pair<Type> {
            if constexpr(!(std::is_integral_v<Type> || std::is_floating_point_v<Type>)) {
                return {.status = literal_status::InvalidLiteralType};
            }
            else {
                Type sum = 0;
                const auto& str = literal.array;
                const auto is_hex
                    = str.size() > 2 && str[0] == '0' && (str[1] == 'x' || str[1] == 'X');

                const auto is_binary
                    = str.size() > 2 && str[0] == '0' && (str[1] == 'b' || str[1] == 'B');

                if constexpr(std::is_floating_point_v<Type>
                             && (std::is_same_v<fmax, f64> || !std::is_same_v<Type, fmax>))
                {
                    if(!is_hex && !is_binary) {
                        constexpr auto trimmed = trim_separators(string_literal<Chars...>{});
                        Type res = 0;
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG
                        const auto begin = &(trimmed[0]);
                        const auto end = &(trimmed[0]) + trimmed.size();
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
    #pragma GCC diagnostic pop
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG
                        auto result = fast_float::from_chars(begin, end, res);
                        if(result.ptr != end) {
                            if(result.ec == std::errc::invalid_argument) {
                                return {.status = literal_status::InvalidCharacterSequence};
                            }

                            return {.status = literal_status::OutOfRange};
                        }

                        return {.status = literal_status::Valid, .value = res};
                    }
                }

                const auto is_octal = str.size() > 1 && str[0] == '0' && !is_hex && !is_binary
                                      && !std::is_floating_point_v<Type>;
                const auto offset = is_hex || is_binary ? 2U : (is_octal ? 1U : 0U);

                constexpr auto to_number = [](char digit) {
                    if(digit >= '0' && digit <= '9') {
                        return static_cast<Type>(digit - '0');
                    }

                    if(digit >= 'a' && digit <= 'f') {
                        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
                        return static_cast<Type>(static_cast<Type>(digit - 'a')
                                                 + static_cast<Type>(10));
                    }

                    if(digit >= 'A' && digit <= 'F') {
                        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
                        return static_cast<Type>(static_cast<Type>(digit - 'A')
                                                 + static_cast<Type>(10));
                    }

                    return static_cast<Type>(0);
                };

                // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
                [[maybe_unused]] const usize base
                    = is_hex ? 16U : (is_binary ? 2U : (is_octal ? 8U : 10U));
                [[maybe_unused]] bool found_decimal = false;
                [[maybe_unused]] usize current_multiplier = 1U;
                [[maybe_unused]] usize num_before_decimal = 0U;
                [[maybe_unused]] usize divisor = 1U;
                const auto size = str.size();
                for(auto i = 0U; i < size - offset; ++i) {
                    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
                    auto& digit = str[size - 1 - i];

                    if constexpr(std::is_floating_point_v<Type>) {
                        if((digit < '0' || digit > '9') && digit != '\'' && digit != '.') {
                            return {.status = literal_status::InvalidCharacterSequence};
                        }
                    }
                    else if(is_octal) {
                        const auto in_0_to_7 = digit >= '0' && digit <= '7';
                        if(!in_0_to_7 && digit != '\'') {
                            return {.status = literal_status::InvalidCharacterSequence};
                        }
                    }
                    else if(!is_binary) {
                        // hex or decimal
                        const auto in_0_to_9 = digit >= '0' && digit <= '9';
                        if(is_hex) {
                            const auto in_a_to_f = digit >= 'a' && digit <= 'f';
                            const auto in_A_to_F = digit >= 'A' && digit <= 'F';
                            const auto valid_hex = (in_a_to_f || in_A_to_F || in_0_to_9);
                            if(!valid_hex && digit != '\'') {
                                return {.status = literal_status::InvalidCharacterSequence};
                            }
                        }
                        else if(!in_0_to_9 && digit != '\'') {
                            return {.status = literal_status::InvalidCharacterSequence};
                        }
                    }
                    else if(!(digit == '0' || digit == '1') && digit != '\'') {
                        return {.status = literal_status::InvalidCharacterSequence};
                    }

                    if constexpr(std::is_floating_point_v<Type>) {
                        if(digit == '.') {
                            found_decimal = true;
                            num_before_decimal = i;
                            for(auto j = 0U; j < num_before_decimal; ++j) {
                                divisor *= base;
                            }
                        }
                        else if(digit != '\'') {
                            Type value = to_number(digit) * static_cast<Type>(current_multiplier);
                            current_multiplier *= base;
                            if(sum / static_cast<Type>(divisor)
                               > std::numeric_limits<Type>::max() - value)
                            {
                                return {.status = literal_status::OutOfRange};
                            }

                            sum += value;
                        }
                    }
                    else {
                        if(digit != '\'') {
                            Type value = to_number(digit) * static_cast<Type>(current_multiplier);
                            current_multiplier *= base;
                            if(sum > std::numeric_limits<Type>::max() - value) {
                                return {.status = literal_status::OutOfRange};
                            }
                            sum += value;
                        }
                    }
                }

                auto result = static_cast<Type>(sum);
                if constexpr(std::is_floating_point_v<Type>) {
                    if(found_decimal && num_before_decimal != 0U) {
                        result /= static_cast<Type>(divisor);
                    }
                }

                return {.status = literal_status::Valid, .value = result};
            }
        }
        HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP
    } // namespace detail

    HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START;

    /// @brief user defined literal for `byte`
    ///
    /// # Example
    /// @code {.cpp}
    /// using hyperion::operator""_byte;
    /// const auto my_byte = 12'345_byte;
    /// @endcode
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    template<char... Chars>
    [[nodiscard]] static constexpr auto operator""_byte() noexcept -> byte { // NOLINT
        constexpr auto parsed = detail::parse_literal<byte>(detail::string_literal<Chars...>());
        detail::check_literal_status<parsed.status>();
        return parsed.value;
    }

    /// @brief user defined literal for `u8`
    ///
    /// # Example
    /// @code {.cpp}
    /// using hyperion::operator""_u8;
    /// const auto my_u8 = 12'345_u8;
    /// @endcode
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    template<char... Chars>
    [[nodiscard]] static constexpr auto operator""_u8() noexcept -> u8 { // NOLINT
        constexpr auto parsed = detail::parse_literal<u8>(detail::string_literal<Chars...>());
        detail::check_literal_status<parsed.status>();
        return parsed.value;
    }

    /// @brief user defined literal for `u16`
    ///
    /// # Example
    /// @code {.cpp}
    /// using hyperion::operator""_u16;
    /// const auto my_u16 = 12'345_16;
    /// @endcode
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    template<char... Chars>
    [[nodiscard]] static constexpr auto operator""_u16() noexcept -> u16 { // NOLINT
        constexpr auto parsed = detail::parse_literal<u16>(detail::string_literal<Chars...>());
        detail::check_literal_status<parsed.status>();
        return parsed.value;
    }

    /// @brief user defined literal for `u32`
    ///
    /// # Example
    /// @code {.cpp}
    /// using hyperion::operator""_u32;
    /// const auto my_u32 = 12'345_u32;
    /// @endcode
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    template<char... Chars>
    [[nodiscard]] static constexpr auto operator""_u32() noexcept -> u32 { // NOLINT
        constexpr auto parsed = detail::parse_literal<u32>(detail::string_literal<Chars...>());
        detail::check_literal_status<parsed.status>();
        return parsed.value;
    }

    /// @brief user defined literal for `u64`
    ///
    /// # Example
    /// @code {.cpp}
    /// using hyperion::operator""_u64;
    /// const auto my_u64 = 12'345_u64;
    /// @endcode
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    template<char... Chars>
    [[nodiscard]] static constexpr auto operator""_u64() noexcept -> u64 { // NOLINT
        constexpr auto parsed = detail::parse_literal<u64>(detail::string_literal<Chars...>());
        detail::check_literal_status<parsed.status>();
        return parsed.value;
    }

    /// @brief user defined literal for `usize`
    ///
    /// # Example
    /// @code {.cpp}
    /// using hyperion::operator""_usize;
    /// const auto my_usize = 12'345_usize;
    /// @endcode
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    template<char... Chars>
    [[nodiscard]] static constexpr auto operator""_usize() noexcept -> usize { // NOLINT
        constexpr auto parsed = detail::parse_literal<usize>(detail::string_literal<Chars...>());
        detail::check_literal_status<parsed.status>();
        return parsed.value;
    }

    /// @brief user defined literal for `umax`
    ///
    /// # Example
    /// @code {.cpp}
    /// using hyperion::operator""_umax;
    /// const auto my_umax = 12'345_umax;
    /// @endcode
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    template<char... Chars>
    [[nodiscard]] static constexpr auto operator""_umax() noexcept -> umax { // NOLINT
        constexpr auto parsed = detail::parse_literal<umax>(detail::string_literal<Chars...>());
        detail::check_literal_status<parsed.status>();
        return parsed.value;
    }

    /// @brief user defined literal for `i8`
    ///
    /// # Example
    /// @code {.cpp}
    /// using hyperion::operator""_i8;
    /// const auto my_i8 = 12'345_i8;
    /// @endcode
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    template<char... Chars>
    [[nodiscard]] static constexpr auto operator""_i8() noexcept -> i8 { // NOLINT
        constexpr auto parsed = detail::parse_literal<i8>(detail::string_literal<Chars...>());
        detail::check_literal_status<parsed.status>();
        return parsed.value;
    }

    /// @brief user defined literal for `i16`
    ///
    /// # Example
    /// @code {.cpp}
    /// using hyperion::operator""_i16;
    /// const auto my_i16 = 12'345_i16;
    /// @endcode
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    template<char... Chars>
    [[nodiscard]] static constexpr auto operator""_i16() noexcept -> i16 { // NOLINT
        constexpr auto parsed = detail::parse_literal<i16>(detail::string_literal<Chars...>());
        detail::check_literal_status<parsed.status>();
        return parsed.value;
    }

    /// @brief user defined literal for `i32`
    ///
    /// # Example
    /// @code {.cpp}
    /// using hyperion::operator""_i32;
    /// const auto my_i32 = 12'345_i32;
    /// @endcode
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    template<char... Chars>
    [[nodiscard]] static constexpr auto operator""_i32() noexcept -> i32 { // NOLINT
        constexpr auto parsed = detail::parse_literal<i32>(detail::string_literal<Chars...>());
        detail::check_literal_status<parsed.status>();
        return parsed.value;
    }

    /// @brief user defined literal for `i64`
    ///
    /// # Example
    /// @code {.cpp}
    /// using hyperion::operator""_i64;
    /// const auto my_i64 = 12'345_i64;
    /// @endcode
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    template<char... Chars>
    [[nodiscard]] static constexpr auto operator""_i64() noexcept -> i64 { // NOLINT
        constexpr auto parsed = detail::parse_literal<i64>(detail::string_literal<Chars...>());
        detail::check_literal_status<parsed.status>();
        return parsed.value;
    }

    /// @brief user defined literal for `imax`
    ///
    /// # Example
    /// @code {.cpp}
    /// using hyperion::operator""_imax;
    /// const auto my_imax = 12'345_imax;
    /// @endcode
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    template<char... Chars>
    [[nodiscard]] static constexpr auto operator""_imax() noexcept -> imax { // NOLINT
        constexpr auto parsed = detail::parse_literal<imax>(detail::string_literal<Chars...>());
        detail::check_literal_status<parsed.status>();
        return parsed.value;
    }

    /// @brief user defined literal for `f32`
    ///
    /// # Example
    /// @code {.cpp}
    /// using hyperion::operator""_f32;
    /// const auto my_f32 = 12'345.6789_f32;
    /// @endcode
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    template<char... Chars>
    [[nodiscard]] static constexpr auto operator""_f32() noexcept -> f32 { // NOLINT
        constexpr auto parsed = detail::parse_literal<f32>(detail::string_literal<Chars...>());
        detail::check_literal_status<parsed.status>();
        return parsed.value;
    }

    /// @brief user defined literal for `f64`
    ///
    /// # Example
    /// @code {.cpp}
    /// using hyperion::operator""_f64;
    /// const auto my_f64 = 12'345.6789_f64;
    /// @endcode
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    template<char... Chars>
    [[nodiscard]] static constexpr auto operator""_f64() noexcept -> f64 { // NOLINT
        constexpr auto parsed = detail::parse_literal<f64>(detail::string_literal<Chars...>());
        detail::check_literal_status<parsed.status>();
        return parsed.value;
    }

    /// @brief user defined literal for `fmax`
    ///
    /// # Example
    /// @code {.cpp}
    /// using hyperion::operator""_fmax;
    /// const auto my_fmax = 12'345.6789_fmax;
    /// @endcode
    /// @ingroup core_numeric_types
    /// @headerfile hyperion/platform/types.h
    template<char... Chars>
    [[nodiscard]] static constexpr auto operator""_fmax() noexcept -> fmax { // NOLINT
        constexpr auto parsed = detail::parse_literal<fmax>(detail::string_literal<Chars...>());
        detail::check_literal_status<parsed.status>();
        return parsed.value;
    }

    HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP;

    namespace literal_tests {
#if HYPERION_PLATFORM_COMPILER_IS_CLANG || HYPERION_PLATFORM_COMPILER_IS_GCC
        _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wfloat-equal\"")
#endif // HYPERION_PLATFORM_COMPILER_CLANG || HYPERION_PLATFORM_COMPILER_GCC
       // clang-format off

        // NOLINTNEXTLINE
        static_assert(static_cast<u8>(8) == 8_u8, "u8 literal operator broken");
        // NOLINTNEXTLINE
        static_assert(static_cast<u16>(8) == 8_u16, "u16 literal operator broken");
		// NOLINTNEXTLINE
		static_assert(static_cast<usize>(64) == 64_usize, "usize literal operator broken!");
		// NOLINTNEXTLINE
		static_assert(static_cast<usize>(64'000) == 64'000_usize, "usize literal operator broken!");
		// NOLINTNEXTLINE
		static_assert(static_cast<usize>(64'123'456) == 64'123'456_usize,
					  "usize literal operator broken!");
		// NOLINTNEXTLINE
		static_assert(static_cast<usize>(0xDEAD'BEEF) == 0xDEAD'BEEF_usize,
					  "usize literal operator broken!");
		// NOLINTNEXTLINE
		static_assert(static_cast<usize>(012345) == 012345_usize,
					  "usize literal operator broken!");
		// NOLINTNEXTLINE
		static_assert(static_cast<usize>(0b0011001100) == 0b0011001100_usize,
					  "usize literal operator broken!");
        // clang-format on

        // NOLINTNEXTLINE
        static_assert(static_cast<i8>(8) == 8_i8, "i8 literal operator broken");
        // NOLINTNEXTLINE
        static_assert(static_cast<i16>(8) == 8_i16, "i16 literal operator broken");
        // NOLINTNEXTLINE
        static_assert(static_cast<i64>(-64'123'456) == -64'123'456_i64,
                      "i64 literal operator broken!");
        // NOLINTNEXTLINE
        static_assert(-static_cast<i64>(0xDEAD'BEEF) == -0xDEAD'BEEF_i64,
                      "i64 literal operator broken!");
        // NOLINTNEXTLINE
        static_assert(-static_cast<i64>(012345) == -012345_i64, "i64 literal operator broken!");
        // NOLINTNEXTLINE
        static_assert(static_cast<i64>(0b0011001100) == 0b0011001100_i64,
                      "i64 literal operator broken!");

        static inline constexpr auto acceptable_deviation
            = static_cast<fmax>(0.000000000001261213356);
        // NOLINTNEXTLINE
        static_assert(static_cast<fmax>(64.123456789) - 64.123456789_fmax < acceptable_deviation,
                      "fmax literal operator broken!");
        // NOLINTNEXTLINE
        static_assert(static_cast<fmax>(64'000) - 64'000_fmax < acceptable_deviation,
                      "fmax literal operator broken!");
        // NOLINTNEXTLINE
        static_assert(static_cast<fmax>(1) - 1_fmax < acceptable_deviation,
                      "fmax literal operator broken!");
        // NOLINTNEXTLINE
        static_assert(static_cast<fmax>(64'000.123456789) - 64'000.123456789_fmax
                          < acceptable_deviation,
                      "fmax literal operator broken!");
        // NOLINTNEXTLINE
        static_assert(static_cast<fmax>(-64'000.123456789) - -64'000.123456789_fmax
                          < acceptable_deviation,
                      "fmax literal operator broken!");
        // NOLINTNEXTLINE
        static_assert(static_cast<fmax>(0.5) - 0.5_fmax < acceptable_deviation,
                      "fmax literal operator broken!");
        // NOLINTNEXTLINE
        static_assert(static_cast<fmax>(0.5) == 0.5_fmax, "fmax literal operator broken!");
        // NOLINTNEXTLINE
        static_assert(static_cast<fmax>(1.0e10) == 10'000'000'000.0_fmax,
                      "fmax literal operator broken!");
        // NOLINTNEXTLINE
        static_assert(static_cast<fmax>(1.0e18) == 1'000'000'000'000'000'000.0_fmax,
                      "fmax literal operator broken!");

#if HYPERION_PLATFORM_COMPILER_IS_CLANG || HYPERION_PLATFORM_COMPILER_IS_GCC
        _Pragma("GCC diagnostic pop")
#endif // HYPERION_PLATFORM_COMPILER_CLANG || HYPERION_PLATFORM_COMPILER_GCC
    } // namespace literal_tests

} // namespace hyperion

#endif // HYPERION_PLATFORM_TYPES_H
