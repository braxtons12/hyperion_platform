/// @file ignore.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Provides utilities to ignore an arbitrary number of values
/// @version 0.4.0
/// @date 2024-06-15
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
#ifndef HYPERION_PLATFORM_IGNORE_H
#define HYPERION_PLATFORM_IGNORE_H

/// @ingroup platform
/// @{
///	@defgroup utility Utilities
/// This module contains basic core library utilities, such as an ergonomic way to
/// silence unused argument warnings and comparison functions that eliminate the pitfalls
/// associated with comparing signed and unsigned types and/or floating point numbers.
/// @}

namespace hyperion {
    /// @brief Utility function to ignore the given values
    ///
    /// # Example
    /// @code{.cpp}
    /// auto my_func(i32 arg1, i32 arg2, i32 arg3) -> void {
    ///     // TODO: Implement this!
    ///     // eliminate unused argument warnings/errors during compilation
    ///     ignore(arg1, arg2, arg3);
    /// }
    /// @endcode
    ///
    /// @tparam Args - The types of the things to ignore
    /// @param args - The things to ignore
    /// @ingroup utility
    /// @headerfile hyperion/platform/ignore.h
    template<typename... Args>
    constexpr auto
    ignore([[maybe_unused]] Args&&... args) // NOLINT(cppcoreguidelines-missing-std-forward)
        noexcept -> void {
        // std::ignore = std::make_tuple<Args...>(std::forward<Args>(args)...);
    }
} // namespace hyperion

#endif // HYPERION_PLATFORM_IGNORE_H
