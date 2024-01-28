/// @file platform.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module includes macro definitions for identifying the platform being compiled for
/// (operating system, compiler, etc)
/// @version 0.1
/// @date 2024-01-26
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

///	@defgroup platform Platform
/// Hyperion provides an assortment of platform detection macros to detect operating system,
/// compiler, little vs big endianness, and architecture.
/// @note while Hyperion provides many different platform detection macros, it has currently only
/// been tested on x86_64 Windows, x86_64 linux, and ARM64 (M1) MacOs
/// @headerfile "hyperion/platform.h"

#ifndef HYPERION_PLATFORM_PLATFORM_H
#define HYPERION_PLATFORM_PLATFORM_H

#include <version>

/// @def HYPERION_IGNORE_UNUSED_MACROS_WARNING_START
/// @brief Use to temporarily disable warnings for unused macros.
/// Make sure to pair with `HYPERION_IGNORE_UNUSED_MACROS_WARNING_STOP` to properly scope the area
/// where the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform.h"
#if !((defined(__MSC_VER) || defined(_MSC_VER)) && !defined(__clang__))
    // NOLINTNEXTLINE
    #define HYPERION_IGNORE_UNUSED_MACROS_WARNING_START \
            _Pragma("GCC diagnostic push")              \
            _Pragma("GCC diagnostic ignored \"-Wunused-macros\"")
#else
    // NOLINTNEXTLINE
    #define HYPERION_IGNORE_UNUSED_MACROS_WARNING_START
#endif // !((defined(__MSC_VER) || defined(_MSC_VER)) && !defined(__clang__))

/// @def HYPERION_IGNORE_UNUSED_MACROS_WARNING_STOP
/// @brief Use to re-enable warnings for unused macros after having previously used
/// `HYPERION_IGNORE_UNUSED_MACROS_WARNING_START`
/// @ingroup defines
/// @headerfile "hyperion/platform.h"
#if !((defined(__MSC_VER) || defined(_MSC_VER)) && !defined(__clang__))
    // NOLINTNEXTLINE
    #define HYPERION_IGNORE_UNUSED_MACROS_WARNING_STOP _Pragma("GCC diagnostic pop")
#else
    // NOLINTNEXTLINE
    #define HYPERION_IGNORE_UNUSED_MACROS_WARNING_STOP
#endif // !((defined(__MSC_VER) || defined(_MSC_VER)) && !defined(__clang__))

HYPERION_IGNORE_UNUSED_MACROS_WARNING_START;
//

/// @def HYPERION_PLATFORM_IS_WINDOWS
/// @brief Whether the compiled-for platform is WINDOWS
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_IS_WINDOWS true
#else
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_IS_WINDOWS false
#endif // defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)

/// @def HYPERION_PLATFORM_IS_APPLE
/// @brief Whether the compiled-for platform is APPLE (MacOS, IOS)
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#if defined(__APPLE__)
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_IS_APPLE true
#else
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_IS_APPLE false
#endif // defined(__APPLE__)

/// @def HYPERION_PLATFORM_IS_UNIX
/// @brief Whether the compiled-for platform is a variant of UNIX
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#if defined(__unix__) || defined(__unix) || defined(unix) \
    || (HYPERION_PLATFORM_IS_APPLE && defined(__MACH__))
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_IS_UNIX true
#else
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_IS_UNIX false
#endif // defined(__unix__) || defined(__unix) || defined(unix)
       // || (HYPERION_PLATFORM_IS_APPLE && defined(__MACH__))

/// @def HYPERION_PLATFORM_IS_LINUX
/// @brief Whether the compiled-for platform is LINUX
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#if defined(linux) || defined(__linux__) || defined(__linux) \
    || (defined(__unix__) && !HYPERION_PLATFORM_IS_APPLE && !defined(BSD))
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_IS_LINUX true
#else
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_IS_LINUX false
#endif // defined(linux) || defined(__linux__) || defined(__linux)
       // || (defined(__unix__) && !HYPERION_PLATFORM_IS_APPLE && !defined(BSD))

/// @def HYPERION_PLATFORM_IS_BSD
/// @brief Whether the compiled-for platform is a BSD
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#if defined(BSD)
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_IS_BSD true
#else
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_IS_BSD false
#endif // defined(BSD)

/// @def HYPERION_PLATFORM_IS_ANDROID
/// @brief Whether the compiled-for platform is Android
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#if defined(__ANDROID__)
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_IS_ANDROID true
#else
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_IS_ANDROID false
#endif // defined(__ANDROID__)

/// @def HYPERION_PLATFORM_COMPILER_IS_CLANG
/// @brief Whether the current compiler is CLANG
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#if defined(__clang__)
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_COMPILER_IS_CLANG true
#else
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_COMPILER_IS_CLANG false
#endif // defined(__clang__)

/// @def HYPERION_PLATFORM_COMPILER_IS_GCC
/// @brief Whether the current compiler is GCC
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#if defined(__GNUC__) && !defined(__clang__)
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_COMPILER_IS_GCC true
#else
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_COMPILER_IS_GCC false
#endif // defined(__GNUC__) && !defined(__clang__)

/// @def HYPERION_PLATFORM_COMPILER_IS_MSVC
/// @brief Whether the current compiler is GCC
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#if(defined(__MSC_VER) || defined(_MSC_VER)) && !HYPERION_PLATFORM_COMPILER_IS_CLANG
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_COMPILER_IS_MSVC true
#else
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_COMPILER_IS_MSVC false
#endif // (defined(__MSC_VER) || defined(_MSC_VER)) && !HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_PLATFORM_STD_LIB_IS_LIBCPP
/// @brief Whether the used standard library is libc++ (llvm)
/// @ingroup platform
/// @headerfile "hyperion/platform.h"

/// @def HYPERION_PLATFORM_STD_LIB_IS_LIBSTDCPP
/// @brief Whether the used standard library is libstdc++ (gcc)
/// @ingroup platform
/// @headerfile "hyperion/platform.h"

/// @def HYPERION_PLATFORM_STD_LIB_IS_MSVC
/// @brief Whether the used standard library is MSVC
/// @ingroup platform
/// @headerfile "hyperion/platform.h"

#if defined(_LIBCPP_VERSION)
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_STD_LIB_IS_LIBCPP true
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_STD_LIB_IS_LIBSTDCPP false
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_STD_LIB_IS_MSVC false
#elif defined(__GLIBCXX__)
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_STD_LIB_IS_LIBCPP    false
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_STD_LIB_IS_LIBSTDCPP true
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_STD_LIB_IS_MSVC      false
#elif defined(_MSC_VER)
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_STD_LIB_IS_LIBCPP    false
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_STD_LIB_IS_LIBSTDCPP false
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_STD_LIB_IS_MSVC      true
#else
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_STD_LIB_IS_LIBCPP    false
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_STD_LIB_IS_LIBSTDCPP false
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_STD_LIB_IS_MSVC      false
#endif // defined(_LIBCPP_VERSION)

/// @def HYPERION_PLATFORM_MODE_IS_DEBUG
/// @brief Whether the current build mode is DEBUG
/// @ingroup platform
/// @headerfile "hyperion/platform.h"

/// @def HYPERION_PLATFORM_MODE_IS_RELEASE
/// @brief Whether the current build mode is RELEASE
/// @ingroup platform
/// @headerfile "hyperion/platform.h"

#if defined(DEBUG) || !defined(NDEBUG)
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_MODE_IS_DEBUG true
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_MODE_IS_RELEASE false
#else
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_MODE_IS_DEBUG   false
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_MODE_IS_RELEASE true
#endif // defined(DEBUG) || !defined(NDEBUG)

/// @brief x86_64 architecture tag
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#define HYPERION_PLATFORM_X86_64 (1U << 1U) // NOLINT
/// @brief x86 architecture tag
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#define HYPERION_PLATFORM_X86 (1U << 2U) // NOLINT
/// @brief ARM v6 architecture tag
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#define HYPERION_PLATFORM_ARM_V6 (1U << 7U) // NOLINT
/// @brief ARM v7 architecture tag
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#define HYPERION_PLATFORM_ARM_V7 (1U << 8U) // NOLINT
/// @brief ARM v7a architecture tag
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#define HYPERION_PLATFORM_ARM_V7A (HYPERION_PLATFORM_ARM_V7 | 1U << 9U) // NOLINT
/// @brief ARM v7r architecture tag
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#define HYPERION_PLATFORM_ARM_V7R /** NOLINT(cppcoreguidelines-macro-usage) **/ \
    (HYPERION_PLATFORM_ARM_V7 | HYPERION_PLATFORM_ARM_V7A | 1U << 10U) // NOLINT
/// @brief ARM v7m architecture tag
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#define HYPERION_PLATFORM_ARM_V7M /** NOLINT(cppcoreguidelines-macro-usage) **/       \
    (HYPERION_PLATFORM_ARM_V7 | HYPERION_PLATFORM_ARM_V7A | HYPERION_PLATFORM_ARM_V7R \
     | 1U << 11U) // NOLINT
/// @brief ARM v7s architecture tag
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#define HYPERION_PLATFORM_ARM_V7S /** NOLINT(cppcoreguidelines-macro-usage) **/       \
    (HYPERION_PLATFORM_ARM_V7 | HYPERION_PLATFORM_ARM_V7A | HYPERION_PLATFORM_ARM_V7R \
     | 1U << 12U) // NOLINT
/// @brief ARM v8 architecture tag
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#define HYPERION_PLATFORM_ARM_V8 (1U << 13U) // NOLINT
/// @brief ARM 64 architecture tag
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#define HYPERION_PLATFORM_ARM_64 (1U << 13U) // NOLINT
/// @brief Unknown architecture tag
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#define HYPERION_PLATFORM_UNKNOWN (1U << 14U) // NOLINT

/// @def HYPERION_PLATFORM_ARCHITECTURE
/// @brief The CPU architecture family being compiled for
///
/// Can be any `HYPERION_PLATFORM_ARCHITECTURE_<ARCH>` tag macro
/// @ingroup platform
/// @headerfile "hyperion/platform.h"

#if defined(__x86_64__) || defined(_M_X64) || defined(__X86_64__)
    #define HYPERION_PLATFORM_ARCHITECTURE HYPERION_PLATFORM_X86_64 // NOLINT
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
    #define HYPERION_PLATFORM_ARCHITECTURE HYPERION_PLATFORM_X86 // NOLINT
#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) \
    || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_ARCHITECTURE HYPERION_PLATFORM_ARM_V6
#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) \
    || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)

    #if defined(__ARM_ARCH_7S__)
        // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
        #define HYPERION_PLATFORM_ARCHITECTURE HYPERION_PLATFORM_ARM_V7S
    #elif defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
        // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
        #define HYPERION_PLATFORM_ARCHITECTURE HYPERION_PLATFORM_ARM_V7M
    #elif defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
        // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
        #define HYPERION_PLATFORM_ARCHITECTURE HYPERION_PLATFORM_ARM_V7R
    #elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) \
        || defined(__ARM_ARCH_7S__)
        // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
        #define HYPERION_PLATFORM_ARCHITECTURE HYPERION_PLATFORM_ARM_V7A
    #else
        // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
        #define HYPERION_PLATFORM_ARCHITECTURE HYPERION_PLATFORM_ARM_V7
    #endif
#elif defined(__aarch64__) || defined(aarch64) || defined(__arm64__) || defined(arm64) \
    || defined(_M_ARM64)
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_ARCHITECTURE HYPERION_PLATFORM_ARM_V8
#else
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_ARCHITECTURE HYPERION_PLATFORM_UNKOWN
#endif // defined(__x86_64__) || defined(_M_X64) || defined(__X86_64__)

/// @brief Determines if the compiled-for architecture is the given one
///
/// This will determine if the architecture being compiled for matches or is a super set of
/// the given architecture to check for.
///
/// @param arch - The platform architecture to check against. Must be one of the enum
/// `HyperionPlatformArchitecture`
///
/// @return whether the compiled-for architecture is the given one
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#define HYPERION_PLATFORM_IS_ARCHITECTURE(arch) /** NOLINT **/ \
    ((HYPERION_PLATFORM_ARCHITECTURE & (arch)) > 0)

// BIG OR LITTLE ENDIAN ?

/// @brief Whether the compiled-for architecture is a little endian architecture
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#define HYPERION_PLATFORM_IS_LITTLE_ENDIAN /** NOLINT(cppcoreguidelines-macro-usage) **/ \
    (!HYPERION_PLATFORM_IS_ARCHITECTURE(HYPERION_PLATFORM_UNKNOWN))
/// @brief Whether the compiled-for architecture is a big endian architecture
/// @ingroup platform
/// @headerfile "hyperion/platform.h"
#define HYPERION_PLATFORM_IS_BIG_ENDIAN /** NOLINT(cppcoreguidelines-macro-usage) **/ \
    HYPERION_PLATFORM_IS_ARCHITECTURE(HYPERION_PLATFORM_UNKNOWN)

/// @def HYPERION_PLATFORM_CACHE_LINE_SIZE
/// @brief The architecture cache-line size
/// @ingroup platform
/// @headerfile "hyperion/platform.h"

#if HYPERION_PLATFORM_IS_ARCHITECTURE(HYPERION_PLATFORM_X86_64) \
    || HYPERION_PLATFORM_IS_ARCHITECTURE(HYPERION_PLATFORM_ARM_V8)
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_CACHE_LINE_SIZE 64
#elif HYPERION_PLATFORM_IS_ARCHITECTURE(HYPERION_PLATFORM_X86)     \
    || HYPERION_PLATFORM_IS_ARCHITECTURE(HYPERION_PLATFORM_ARM_V7) \
    || HYPERION_PLATFORM_IS_ARCHITECTURE(HYPERION_PLATFORM_ARM_V6)
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_CACHE_LINE_SIZE 32
#else
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define HYPERION_PLATFORM_CACHE_LINE_SIZE 128
#endif // HYPERION_PLATFORM_IS_ARCHITECTURE(HYPERION_PLATFORM_X86_64)
       // || HYPERION_PLATFORM_IS_ARCHITECTURE(HYPERION_PLATFORM_ARM_V8)

HYPERION_IGNORE_UNUSED_MACROS_WARNING_STOP

#endif // HYPERION_PLATFORM_PLATFORM_H
