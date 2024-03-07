/// @file def.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Provides various macro definitions for things like compiler-specific attributes,
/// feature enablement, and warning suppression
/// @version 0.2.4
/// @date 2024-03-06
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

/// @ingroup platform
/// @{
///	@defgroup defines Macro Definitions
/// This module provides a collection of macro definitions for things like compiler-specific
/// attributes, feature inclusion, and warning suppression.
/// @headerfile "hyperion/platform/def.h"
/// @}

#ifndef HYPERION_PLATFORM_DEF_H
#define HYPERION_PLATFORM_DEF_H

#include <hyperion/platform.h>
#include <version>

HYPERION_IGNORE_UNUSED_MACROS_WARNING_START;
//

/// @def HYPERION_STD_LIB_HAS_EXPERIMENTAL_SOURCE_LOCATION
/// @brief Whether Hyperion will use `std::experimental::source_location` if `<source_location>`
/// is not yet available for the given compiler. This defaults to true when using libstdc++
/// (gcc's std lib). To disable it, define it to false prior to including any Hyperion headers
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if !defined(HYPERION_STD_LIB_HAS_EXPERIMENTAL_SOURCE_LOCATION)
    #if __has_include(<experimental/source_location>)
        #define HYPERION_STD_LIB_HAS_EXPERIMENTAL_SOURCE_LOCATION true
    #else // !__has_include(<experimental/source_location>)
        #define HYPERION_STD_LIB_HAS_EXPERIMENTAL_SOURCE_LOCATION false
    #endif // __has_include(<experimental/source_location>)
#endif     // !defined(HYPERION_STD_LIB_HAS_EXPERIMENTAL_SOURCE_LOCATION)

/// @def HYPERION_STD_LIB_HAS_SOURCE_LOCATION
/// @brief if `<source_location>` is available, this will be defined as true, otherwise it will be
/// false. If this is false, `std::source_location` may be aliased as
/// `std::experimental::source_location` if `HYPERION_STD_LIB_HAS_EXPERIMENTAL_SOURCE_LOCATION` is
/// true, otherwise, features relying on `std::source_location` will revert to macros using
/// `__FILE__` and
/// `__LINE__` instead
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if __has_include(<source_location>)
    #if HYPERION_PLATFORM_COMPILER_IS_GCC \
        || (HYPERION_PLATFORM_COMPILER_IS_CLANG && !HYPERION_PLATFORM_IS_WINDOWS)
        #if defined(__has_builtin)
            // NOLINTNEXTLINE(readability-redundant-preprocessor)
            #if __has_builtin(__builtin_source_location)
                #define HYPERION_STD_LIB_HAS_SOURCE_LOCATION true
            #else // !__has_builtin(__builtin_source_location)
                #define HYPERION_STD_LIB_HAS_SOURCE_LOCATION false
            #endif // __has_builtin(__builtin_source_location)
        #elif HYPERION_PLATFORM_STD_LIB_LIBCPP
            #define HYPERION_STD_LIB_HAS_SOURCE_LOCATION true
        #else
            #define HYPERION_STD_LIB_HAS_SOURCE_LOCATION false
        #endif // __has_builtin(__builtin_source_location)
    #else      // MSVC
        #define HYPERION_STD_LIB_HAS_SOURCE_LOCATION true
    #endif // HYPERION_PLATFORM_COMPILER_IS_GCC
           // || (HYPERION_PLATFORM_COMPILER_IS_CLANG && !HYPERION_PLATFORM_IS_WINDOWS)
#else
    #define HYPERION_STD_LIB_HAS_SOURCE_LOCATION false
#endif // (defined(__cpp_lib_source_location) && __cpp_lib_source_location >= 201907)

/// @def HYPERION_STD_LIB_HAS_JTHREAD
/// @brief if `std::jthread` is available, this will be defined as true, otherwise it will be false.
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if(defined(__cpp_lib_jthread) && __cpp_lib_jthread >= 201911L)
    #define HYPERION_STD_LIB_HAS_JTHREAD true
#else
    #define HYPERION_STD_LIB_HAS_JTHREAD false
#endif // (defined(__cpp_lib_jthread) && __cpp_lib_jthread >= 201911L)

/// @def HYPERION_PLATFORM_STD_LIB_HAS_COMPARE
/// @brief Used to check if the compiled-against standard library supports
/// `#include <compare>`
/// @link [__cpp_concepts](https://en.cppreference.com/w/cpp/feature_test)
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if __cpp_lib_three_way_comparison >= 201907L
    #define HYPERION_PLATFORM_STD_LIB_HAS_COMPARE true
#else
    #define HYPERION_PLATFORM_STD_LIB_HAS_COMPARE false
#endif // __cpp_lib_three_way_comparison >= 201907L

/// @def HYPERION_COMPILER_HAS_TYPE_PACK_ELEMENT
/// @brief Indicates whether the compiler builtin `__type_pack_element` is available
/// (used for efficient type parameter pack indexing in `mpl`)
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if defined(__has_builtin)
    #if __has_builtin(__type_pack_element)
        #define HYPERION_COMPILER_HAS_TYPE_PACK_ELEMENT true
    #else // !__has_builtin(__type_pack_element)
        #define HYPERION_COMPILER_HAS_TYPE_PACK_ELEMENT false
    #endif // __has_builtin(__type_pack_element)
#else
    #define HYPERION_COMPILER_HAS_TYPE_PACK_ELEMENT false
#endif // defined(__has_builtin)

/// @def HYPERION_PLATFORM_COMPILER_SUPPORTS_CONDITIONALLY_TRIVIAL_SMFS
/// @brief Used to check if the current compiler supports conditionally trivial
/// special member functions
/// @link [__cpp_concepts](https://en.cppreference.com/w/cpp/feature_test)
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if defined(__cpp_concepts) && __cpp_concepts >= 202002L
    #define HYPERION_PLATFORM_COMPILER_SUPPORTS_CONDITIONALLY_TRIVIAL_SMFS true
#else
    #define HYPERION_PLATFORM_COMPILER_SUPPORTS_CONDITIONALLY_TRIVIAL_SMFS false
#endif // defined(__cpp_concepts) && __cpp_concepts >= 202002L

/// @def HYPERION_NO_UNIQUE_ADDRESS
/// @brief Platform-dependent, conditional [[no_unique_address]] to account for:
/// 	1. MSVC dragging their feet on providing an actual implementation
/// 	2. Clang not supporting it on Windows because of MSVC's lack of support
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_MSVC
    #define HYPERION_NO_UNIQUE_ADDRESS msvc::no_unique_address
#elif HYPERION_PLATFORM_IS_WINDOWS && HYPERION_PLATFORM_COMPILER_IS_CLANG
    #define HYPERION_NO_UNIQUE_ADDRESS
#else
    #define HYPERION_NO_UNIQUE_ADDRESS no_unique_address
#endif // HYPERION_PLATFORM_COMPILER_IS_MSVC

/// @def HYPERION_TRIVIAL_ABI
/// @brief Use to apply clang's `trivial_abi` attribute to the following class/struct declaration
/// when compiling with clang. On other compilers this macro is empty
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
    #define HYPERION_TRIVIAL_ABI [[clang::trivial_abi]]
#else
    #define HYPERION_TRIVIAL_ABI
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_NO_DESTROY
/// @brief Use to apply clang's `no_destroy` attribute to the following variable declaration
/// when compiling with clang. On other compilers this macro is empty
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
    #define HYPERION_NO_DESTROY [[clang::no_destroy]] // NOLINT
#else
    #define HYPERION_NO_DESTROY
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_CONSTEXPR_STRINGS
/// @brief Conditionally marks a function or variable constexpr if `constexpr std::string` has been
/// implemented for the compiler/standard library implementation being compiled with
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if defined(__cpp_lib_constexpr_string)
    #if(__cpp_lib_constexpr_string >= 201907L)
        #define HYPERION_CONSTEXPR_STRINGS constexpr
    #else
        #define HYPERION_CONSTEXPR_STRINGS
    #endif
#else
    #define HYPERION_CONSTEXPR_STRINGS
#endif // defined(__cpp_lib_constexpr_string)

/// @def HYPERION_UNREACHABLE()
/// @brief Marks the containing control flow branch as unreachable. On GCC/Clang, this will use
/// `__builtin_unreachable()`, on MSVC `__assume(false)`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG || HYPERION_PLATFORM_COMPILER_IS_GCC
    #define HYPERION_UNREACHABLE() __builtin_unreachable() // NOLINT(cppcoreguidelines-macro-usage
#elif HYPERION_PLATFORM_COMPILER_IS_MSVC
    #define HYPERION_UNREACHABLE() __assume(false)
#else
    #define HYPERION_UNREACHABLE()
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG || HYPERION_PLATFORM_COMPILER_IS_GCC

// clang-format off

/// @def HYPERION_IGNORE_SUGGEST_DESTRUCTOR_OVERRIDE_WARNING_START
/// @brief Use to temporarily disable warnings for destructors that override but are not marked
/// `override` (-Wsuggest-destructor-override).
/// Make sure to pair with `HYPERION_IGNORE_SUGGEST_DESTRUCTOR_OVERRIDE_WARNING_STOP` to properly scope the area
/// where the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
    // NOLINTNEXTLINE
    #define HYPERION_IGNORE_SUGGEST_DESTRUCTOR_OVERRIDE_WARNING_START \
            _Pragma("GCC diagnostic push")                           \
            _Pragma("GCC diagnostic ignored \"-Wsuggest-destructor-override\"")
#else
// NOLINTNEXTLINE
    #define HYPERION_IGNORE_SUGGEST_DESTRUCTOR_OVERRIDE_WARNING_START
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_SUGGEST_DESTRUCTOR_OVERRIDE_WARNING_STOP
/// @brief Use to re-enable warnings for destructors that override but are not marked `override`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
    // NOLINTNEXTLINE
    #define HYPERION_IGNORE_SUGGEST_DESTRUCTOR_OVERRIDE_WARNING_STOP \
            _Pragma("GCC diagnostic pop")
#else
    // NOLINTNEXTLINE
	#define HYPERION_IGNORE_SUGGEST_DESTRUCTOR_OVERRIDE_WARNING_STOP
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_SWITCH_MISSING_ENUM_VALUES_WARNING_START
/// @brief Use to temporarily disable warnings for switches that don't cover all enum values of the
/// enum being "switched" on (-Wswitch-enum).
/// Make sure to pair with `HYPERION_IGNORE_SWITCH_MISSING_ENUM_VALUES_WARNING_STOP` to properly scope the area
/// where the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
    // NOLINTNEXTLINE
    #define HYPERION_IGNORE_SWITCH_MISSING_ENUM_VALUES_WARNING_START \
            _Pragma("GCC diagnostic push")                          \
            _Pragma("GCC diagnostic ignored \"-Wswitch-enum\"")
#else
    // NOLINTNEXTLINE
    #define HYPERION_IGNORE_SWITCH_MISSING_ENUM_VALUES_WARNING_START
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_SWITCH_MISSING_ENUM_VALUES_WARNING_STOP
/// @brief Use to re-enable warnings for switches that don't cover all enum values of the enum being
/// "switched" on (-Wswitch-enum)
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
    // NOLINTNEXTLINE
    #define HYPERION_IGNORE_SWITCH_MISSING_ENUM_VALUES_WARNING_STOP \
            _Pragma("GCC diagnostic pop")
#else
    // NOLINTNEXTLINE
    #define HYPERION_IGNORE_SWITCH_MISSING_ENUM_VALUES_WARNING_STOP
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_OLD_STYLE_CASTS_WARNING_START
/// @brief Use to temporarily disable warnings for c-style casts (-Wold-style-cast).
/// Make sure to pair with `HYPERION_IGNORE_OLD_STYLE_CASTS_WARNING_START` to properly scope the area where
/// the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
    // NOLINTNEXTLINE
    #define HYPERION_IGNORE_OLD_STYLE_CASTS_WARNING_START \
            _Pragma("GCC diagnostic push")               \
            _Pragma("GCC diagnostic ignored \"-Wold-style-cast\"")
#else
    // NOLINTNEXTLINE
    #define HYPERION_IGNORE_OLD_STYLE_CASTS_WARNING_START
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_OLD_STYLE_CASTS_WARNING_STOP
/// @brief Use to re-enable warnings for c-style casts
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
    // NOLINTNEXTLINE
    #define HYPERION_IGNORE_OLD_STYLE_CASTS_WARNING_STOP \
		    _Pragma("GCC diagnostic pop")
#else
// NOLINTNEXTLINE
	#define HYPERION_IGNORE_OLD_STYLE_CASTS_WARNING_STOP
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_INVALID_NORETURN_WARNING_START
/// @brief Use to temporarily disable warnings for functions marked `[[noreturn]]` that may return
/// (-Winvalid-noreturn).
/// Make sure to pair with `HYPERION_IGNORE_INVALID_NORETURN_WARNING_STOP` to properly scope the area where
/// the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
    #define HYPERION_IGNORE_INVALID_NORETURN_WARNING_START \
	        _Pragma("GCC diagnostic push")                \
	        _Pragma("GCC diagnostic ignored \"-Winvalid-noreturn\"")
#else
    // NOLINTNEXTLINE
    #define HYPERION_IGNORE_INVALID_NORETURN_WARNING_START
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_INVALID_NORETURN_WARNING_STOP
/// @brief Use to re-enable warnings for for functions marked `[[noreturn]]` that may return
/// `HYPERION_IGNORE_INVALID_NORETURN_WARNING_START`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
    // NOLINTNEXTLINE
    #define HYPERION_IGNORE_INVALID_NORETURN_WARNING_STOP \
		    _Pragma("GCC diagnostic pop")
#else
    // NOLINTNEXTLINE
	#define HYPERION_IGNORE_INVALID_NORETURN_WARNING_STOP
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_DEPRECATED_DECLARATIONS_WARNING_START
/// @brief Use to temporarily disable warnings for using deprecated declarations
/// (-Wdeprecated-declarations).
/// Make sure to pair with `HYPERION_IGNORE_DEPRECATED_DECLARATIONS_WARNING_STOP` to properly scope the area where
/// the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_DEPRECATED_DECLARATIONS_WARNING_START \
		    _Pragma("GCC diagnostic push")                       \
		    _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_DEPRECATED_DECLARATIONS_WARNING_START
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_DEPRECATED_DECLARATIONS_WARNING_STOP
/// @brief Use to re-enable warnings for using deprecated declarations after having previously used
/// `HYPERION_IGNORE_DEPRECATED_DECLARATIONS_WARNING_START`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_DEPRECATED_DECLARATIONS_WARNING_STOP \
		    _Pragma("GCC diagnostic pop")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_DEPRECATED_DECLARATIONS_WARNING_STOP
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_UNINITIALIZED_VARIABLES_WARNING_START
/// @brief Use to temporarily disable warnings for using uninitialized variables
/// (-Wuninitialized).
/// Make sure to pair with `HYPERION_IGNORE_UNINITIALIZED_VARIABLES_WARNING_STOP` to properly scope the area where
/// the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNINITIALIZED_VARIABLES_WARNING_START \
		    _Pragma("GCC diagnostic push")                       \
		    _Pragma("GCC diagnostic ignored \"-Wuninitialized\"")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNINITIALIZED_VARIABLES_WARNING_START
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_UNINITIALIZED_VARIABLES_WARNING_STOP
/// @brief Use to re-enable warnings for using uninitialized variables after having previously used
/// `HYPERION_IGNORE_UNINITIALIZED_VARIABLES_WARNING_START`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNINITIALIZED_VARIABLES_WARNING_STOP \
		    _Pragma("GCC diagnostic pop")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNINITIALIZED_VARIABLES_WARNING_STOP
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_UNNEEDED_INTERNAL_DECL_WARNING_START
/// @brief Use to temporarily disable warnings for unneeded internal declarations
/// (-Wunneeded-internal-declaration).
/// Make sure to pair with `HYPERION_IGNORE_UNNEEDED_INTERNAL_DECL_WARNING_STOP` to properly scope the area where
/// the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNNEEDED_INTERNAL_DECL_WARNING_START \
		    _Pragma("GCC diagnostic push")                      \
		    _Pragma("GCC diagnostic ignored \"-Wunneeded-internal-declaration\"")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNNEEDED_INTERNAL_DECL_WARNING_START
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_UNNEEDED_INTERNAL_DECL_WARNING_STOP
/// @brief Use to re-enable warnings for unneeded internal declarations after having previously used
/// `HYPERION_IGNORE_UNNEEDED_INTERNAL_DECL_WARNING_START`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNNEEDED_INTERNAL_DECL_WARNING_STOP \
		    _Pragma("GCC diagnostic pop")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNNEEDED_INTERNAL_DECL_WARNING_STOP
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_CONSTANT_CONDITIONAL_EXPRESSIONS_WARNING_START
/// @brief Use to temporarily disable warnings for constant conditional expressions in non-constexpr
/// if (MSVC 4127).
/// Make sure to pair with `HYPERION_IGNORE_CONSTANT_CONDITIONAL_EXPRESSIONS_WARNING_STOP` to properly scope the
/// area where the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if !HYPERION_PLATFORM_COMPILER_IS_MSVC
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_CONSTANT_CONDITIONAL_EXPRESSIONS_WARNING_START
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_CONSTANT_CONDITIONAL_EXPRESSIONS_WARNING_START \
		    _Pragma("warning( push )")                                    \
		    _Pragma("warning( disable : 4127 )")
#endif // !HYPERION_PLATFORM_COMPILER_IS_MSVC

/// @def HYPERION_IGNORE_CONSTANT_CONDITIONAL_EXPRESSIONS_WARNING_STOP
/// @brief Use to re-enable warnings for constant conditional expressions after having previously
/// used `HYPERION_IGNORE_CONSTANT_CONDITIONAL_EXPRESSIONS_WARNING_STOP`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if !HYPERION_PLATFORM_COMPILER_IS_MSVC
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_CONSTANT_CONDITIONAL_EXPRESSIONS_WARNING_STOP
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_CONSTANT_CONDITIONAL_EXPRESSIONS_WARNING_STOP \
		    _Pragma("warning( pop )")
#endif // !HYPERION_PLATFORM_COMPILER_IS_MSVC

/// @def HYPERION_IGNORE_MARKED_NOEXCEPT_BUT_THROWS_WARNING_START
/// @brief Use to temporarily disable warnings for functions marked noexcept that intentionally
/// throw (MSVC 4297).
/// Make sure to pair with `HYPERION_IGNORE_MARKED_NOEXCEPT_BUT_THROWS_WARNING_STOP` to properly scope the
/// area where the warning is ignored
/// @note Using this __**ONLY**__ makes sense if it's used to suppress __warnings__ when throwing is
/// intentionally used as a way to force compiler errors in constexpr functions
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if !HYPERION_PLATFORM_COMPILER_IS_MSVC
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_MARKED_NOEXCEPT_BUT_THROWS_WARNING_START
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_MARKED_NOEXCEPT_BUT_THROWS_WARNING_START \
		    _Pragma("warning( push )")                              \
		    _Pragma("warning( disable : 4297 )")
#endif // !HYPERION_PLATFORM_COMPILER_IS_MSVC

/// @def HYPERION_IGNORE_MARKED_NOEXCEPT_BUT_THROWS_WARNING_STOP
/// @brief Use to re-enable warnings for functions marked noexcept that intentionally throw after
/// having previously used `HYPERION_IGNORE_MARKED_NOEXCEPT_BUT_THROWS_WARNING_START`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if !HYPERION_PLATFORM_COMPILER_IS_MSVC
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_MARKED_NOEXCEPT_BUT_THROWS_WARNING_STOP
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_MARKED_NOEXCEPT_BUT_THROWS_WARNING_STOP \
		    _Pragma("warning( pop )")
#endif // !HYPERION_PLATFORM_COMPILER_IS_MSVC

/// @def HYPERION_IGNORE_UNUSED_VALUES_WARNING_START
/// @brief Use to temporarily disable warnings for unused variables
/// Make sure to pair with `HYPERION_IGNORE_UNUSED_VALUES_WARNING_STOP` to properly scope the
/// area where the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG || HYPERION_PLATFORM_COMPILER_IS_GCC
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNUSED_VALUES_WARNING_START \
		    _Pragma("GCC diagnostic push")             \
		    _Pragma("GCC diagnostic ignored \"-Wunused-value\"")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNUSED_VALUES_WARNING_START
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG || HYPERION_PLATFORM_COMPILER_IS_GCC

/// @def HYPERION_IGNORE_UNUSED_VALUES_WARNING_STOP
/// @brief Use to re-enable warnings for unused variables after having previously used
/// `HYPERION_IGNORE_UNUSED_VALUES_WARNING_START`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG || HYPERION_PLATFORM_COMPILER_IS_GCC
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNUSED_VALUES_WARNING_STOP \
		    _Pragma("GCC diagnostic pop")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNUSED_VALUES_WARNING_STOP
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG || HYPERION_PLATFORM_COMPILER_IS_GCC

/// @def HYPERION_IGNORE_UNUSED_VARIABLES_WARNING_START
/// @brief Use to temporarily disable warnings for unused variables
/// Make sure to pair with `HYPERION_IGNORE_UNUSED_VARIABLES_WARNING_STOP` to properly scope the
/// area where the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if !HYPERION_PLATFORM_COMPILER_IS_MSVC
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNUSED_VARIABLES_WARNING_START \
		    _Pragma("GCC diagnostic push")                \
		    _Pragma("GCC diagnostic ignored \"-Wunused-variable\"")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNUSED_VARIABLES_WARNING_START
#endif // !HYPERION_PLATFORM_COMPILER_IS_MSVC

/// @def HYPERION_IGNORE_UNUSED_VARIABLES_WARNING_STOP
/// @brief Use to re-enable warnings for unused variables after having previously used
/// `HYPERION_IGNORE_UNUSED_VARIABLES_WARNING_START`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if !HYPERION_PLATFORM_COMPILER_IS_MSVC
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNUSED_VARIABLES_WARNING_STOP \
		    _Pragma("GCC diagnostic pop")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNUSED_VARIABLES_WARNING_STOP
#endif // !HYPERION_PLATFORM_COMPILER_IS_MSVC

/// @def HYPERION_IGNORE_MISSING_NORETURN_WARNING_START
/// @brief Use to temporarily disable warnings for noreturn functions missing a noreturn attribute
/// Make sure to pair with `HYPERION_IGNORE_MISSING_NORETURN_WARNING_STOP` to properly scope the
/// area where the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if !HYPERION_PLATFORM_COMPILER_IS_MSVC
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_MISSING_NORETURN_WARNING_START \
		    _Pragma("GCC diagnostic push")                \
		    _Pragma("GCC diagnostic ignored \"-Wmissing-noreturn\"")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_MISSING_NORETURN_WARNING_START
#endif // !HYPERION_PLATFORM_COMPILER_IS_MSVC

/// @def HYPERION_IGNORE_MISSING_NORETURN_WARNING_STOP
/// @brief Use to re-enable warnings for noreturn functions missing a noreturn attribute after
/// having previously used `HYPERION_IGNORE_MISSING_NORETURN_WARNING_START`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if !HYPERION_PLATFORM_COMPILER_IS_MSVC
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_MISSING_NORETURN_WARNING_STOP \
		    _Pragma("GCC diagnostic pop")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_MISSING_NORETURN_WARNING_STOP
#endif // !HYPERION_PLATFORM_COMPILER_IS_MSVC

/// @def HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START
/// @brief Use to temporarily disable warnings using reserved identifiers.
/// Make sure to pair with `HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP` to properly scope the
/// area where the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START \
		    _Pragma("GCC diagnostic push")                    \
		    _Pragma("GCC diagnostic ignored \"-Wreserved-identifier\"")
#elif HYPERION_PLATFORM_COMPILER_IS_MSVC
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START \
		    _Pragma("warning( push )")                        \
		    _Pragma("warning( disable : 4405 )")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP
/// @brief Use to re-enable warnings for using reserved identifiers after having previously used
/// `HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP \
		    _Pragma("GCC diagnostic pop")
#elif HYPERION_PLATFORM_COMPILER_IS_MSVC
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP \
		    _Pragma("warning( pop )")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_RESERVED_MACRO_IDENTIFIERS_WARNING_START
/// @brief Use to temporarily disable warnings using reserved macro identifiers.
/// Make sure to pair with `HYPERION_IGNORE_RESERVED_MACRO_IDENTIFIERS_WARNING_STOP` to properly scope the
/// area where the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_RESERVED_MACRO_IDENTIFIERS_WARNING_START \
		    _Pragma("GCC diagnostic push")                          \
		    _Pragma("GCC diagnostic ignored \"-Wreserved-macro-identifier\"")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_RESERVED_MACRO_IDENTIFIERS_WARNING_START
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_RESERVED_MACRO_IDENTIFIERS_WARNING_STOP
/// @brief Use to re-enable warnings for using reserved macro identifiers after having previously
/// used `HYPERION_IGNORE_RESERVED_MACRO_IDENTIFIERS_WARNING_START`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_RESERVED_MACRO_IDENTIFIERS_WARNING_STOP \
		    _Pragma("GCC diagnostic pop")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_RESERVED_MACRO_IDENTIFIERS_WARNING_STOP
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_PADDING_WARNING_START
/// @brief Use to temporarily disable warnings for class/struct definitions requiring padding.
/// Make sure to pair with `HYPERION_IGNORE_PADDING_WARNING_STOP` to properly scope the
/// area where the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if !HYPERION_PLATFORM_COMPILER_IS_MSVC
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_PADDING_WARNING_START \
		    _Pragma("GCC diagnostic push")       \
		    _Pragma("GCC diagnostic ignored \"-Wpadded\"")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_PADDING_WARNING_START \
		_Pragma("warning( push )") \
		_Pragma("warning( disable : 4820 )")
#endif // !HYPERION_PLATFORM_COMPILER_IS_MSVC

/// @def HYPERION_IGNORE_PADDING_WARNING_STOP
/// @brief Use to re-enable warnings for class/struct definitions requiring padding after having
/// previously used `HYPERION_IGNORE_PADDING_WARNING_START`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if !HYPERION_PLATFORM_COMPILER_IS_MSVC
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_PADDING_WARNING_STOP \
		    _Pragma("GCC diagnostic pop")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_PADDING_WARNING_STOP \
		    _Pragma("warning( pop )")
#endif // !HYPERION_PLATFORM_COMPILER_IS_MSVC

/// @def HYPERION_IGNORE_WEAK_VTABLES_WARNING_START
/// @brief Use to temporarily disable warnings for virtual classes with weak vtables.
/// Make sure to pair with `HYPERION_IGNORE_WEAK_VTABLES_WARNING_STOP` to properly scope the
/// area where the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
		// NOLINTNEXTLINE
		#define HYPERION_IGNORE_WEAK_VTABLES_WARNING_START \
			    _Pragma("GCC diagnostic push")            \
			    _Pragma("GCC diagnostic ignored \"-Wweak-vtables\"")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_WEAK_VTABLES_WARNING_START
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_WEAK_VTABLES_WARNING_STOP
/// @brief Use to re-enable warnings for virtual classes with weak vtables after having
/// previously used `HYPERION_IGNORE_WEAK_VTABLES_WARNING_START`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
		// NOLINTNEXTLINE
		#define HYPERION_IGNORE_WEAK_VTABLES_WARNING_STOP \
			    _Pragma("GCC diagnostic pop")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_WEAK_VTABLES_WARNING_STOP
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START
/// @brief Use to temporarily disable warnings for unused function templates
/// Make sure to pair with `HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP` to properly scope the
/// area where the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START \
		    _Pragma("GCC diagnostic push")                \
		    _Pragma("GCC diagnostic ignored \"-Wunused-template\"")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP
/// @brief Use to re-enable warnings for unused function templates after having
/// previously used `HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_START`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP \
		    _Pragma("GCC diagnostic pop")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNUSED_TEMPLATES_WARNING_STOP
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_UNUSED_FUNCTIONS_WARNING_START
/// @brief Use to temporarily disable warnings for unused functions
/// Make sure to pair with `HYPERION_IGNORE_UNUSED_FUNCTIONS_WARNING_STOP` to properly scope the
/// area where the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNUSED_FUNCTIONS_WARNING_START \
		    _Pragma("GCC diagnostic push")                \
		    _Pragma("GCC diagnostic ignored \"-Wunused-function\"")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNUSED_FUNCTIONS_WARNING_START
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_UNUSED_FUNCTIONS_WARNING_STOP
/// @brief Use to re-enable warnings for unused functions after having
/// previously used `HYPERION_IGNORE_UNUSED_FUNCTIONS_WARNING_START`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNUSED_FUNCTIONS_WARNING_STOP \
		    _Pragma("GCC diagnostic pop")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNUSED_FUNCTIONS_WARNING_STOP
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_UNUSED_MEMBER_FUNCTIONS_WARNING_START
/// @brief Use to temporarily disable warnings for unused member functions
/// Make sure to pair with `HYPERION_IGNORE_UNUSED_MEMBER_FUNCTIONS_WARNING_STOP` to properly scope the
/// area where the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNUSED_MEMBER_FUNCTIONS_WARNING_START \
		    _Pragma("GCC diagnostic push")                       \
		    _Pragma("GCC diagnostic ignored \"-Wunused-member-function\"")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNUSED_MEMBER_FUNCTIONS_WARNING_START
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_UNUSED_MEMBER_FUNCTIONS_WARNING_STOP
/// @brief Use to re-enable warnings for unused member functions after having
/// previously used `HYPERION_IGNORE_UNUSED_MEMBER_FUNCTIONS_WARNING_START`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNUSED_MEMBER_FUNCTIONS_WARNING_STOP \
		    _Pragma("GCC diagnostic pop")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNUSED_MEMBER_FUNCTIONS_WARNING_STOP
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_SIGNED_BITFIELD_WARNING_START
/// @brief Use to temporarily disable warnings for enums with signed underlying type used as
/// bitfields
/// Make sure to pair with `HYPERION_IGNORE_SIGNED_BITFIELD_WARNING_STOP` to properly scope the
/// area where the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_SIGNED_BITFIELD_WARNING_START \
		    _Pragma("GCC diagnostic push")               \
		    _Pragma("GCC diagnostic ignored \"-Wsigned-enum-bitfield\"")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_SIGNED_BITFIELD_WARNING_START
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_SIGNED_BITFIELD_WARNING_STOP
/// @brief Use to re-enable warnings for enums with signed underlying type used as bitfields after
/// having previously used `HYPERION_IGNORE_SIGNED_BITFIELD_WARNING_START`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_SIGNED_BITFIELD_WARNING_STOP \
		    _Pragma("GCC diagnostic pop")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_SIGNED_BITFIELD_WARNING_STOP
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_UNKNOWN_DOC_COMMAND_WARNING_START
/// @brief Use to temporarily disable warnings unknown documentation commands
/// Make sure to pair with `HYPERION_IGNORE_UNKNOWN_DOC_COMMAND_WARNING_STOP` to properly scope the
/// area where the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNKNOWN_DOC_COMMAND_WARNING_START \
		    _Pragma("GCC diagnostic push")                   \
		    _Pragma("GCC diagnostic ignored \"-Wdocumentation-unknown-command\"")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNKNOWN_DOC_COMMAND_WARNING_START
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_UNKNOWN_DOC_COMMAND_WARNING_STOP
/// @brief Use to re-enable warnings for unknown documentation commands after having previously used
/// `HYPERION_IGNORE_UNKNOWN_DOC_COMMAND_WARNING_START`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNKNOWN_DOC_COMMAND_WARNING_STOP \
		    _Pragma("GCC diagnostic pop")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_UNKNOWN_DOC_COMMAND_WARNING_STOP
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_DOCUMENTATION_WARNING_START
/// @brief Use to temporarily disable warnings about documentation
/// (for example, using a tparam directive in a non-template)
/// Make sure to pair with `HYPERION_IGNORE_DOCUMENTATION_WARNING_STOP` to properly scope the
/// area where the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_DOCUMENTATION_WARNING_START \
		    _Pragma("GCC diagnostic push")             \
		    _Pragma("GCC diagnostic ignored \"-Wdocumentation\"")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_DOCUMENTATION_WARNING_START
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_DOCUMENTATION_WARNING_STOP
/// @brief Use to re-enable warnings about documentation (for example, using a
/// tparam directive in a non-template) after having previously used
/// `HYPERION_IGNORE_DOCUMENTATION_WARNING_START`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_DOCUMENTATION_WARNING_STOP \
		    _Pragma("GCC diagnostic pop")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_DOCUMENTATION_WARNING_STOP
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_CONSTRUCTOR_SHADOW_FIELDS_WARNING_START
/// @brief Use to temporarily disable warnings for constructor parameters that shadow class/struct
/// members
/// Make sure to pair with `HYPERION_IGNORE_CONSTRUCTOR_SHADOW_FIELDS_WARNING_STOP` to properly scope the
/// area where the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_CONSTRUCTOR_SHADOW_FIELDS_WARNING_START \
		    _Pragma("GCC diagnostic push")                         \
		    _Pragma("GCC diagnostic ignored \"-Wshadow-field-in-constructor\"")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_CONSTRUCTOR_SHADOW_FIELDS_WARNING_START
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_CONSTRUCTOR_SHADOW_FIELDS_WARNING_STOP
/// @brief Use to re-enable warnings for constructor parameters that shadow class/struct members
/// after having previously used `HYPERION_IGNORE_CONSTRUCTOR_SHADOW_FIELDS_WARNING_STOP`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_CONSTRUCTOR_SHADOW_FIELDS_WARNING_STOP \
		    _Pragma("GCC diagnostic pop")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_CONSTRUCTOR_SHADOW_FIELDS_WARNING_STOP
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_FLOAT_EQUALITY_WARNING_START
/// @brief Use to temporarily disable warnings for checking for floating point equality
/// Make sure to pair with `HYPERION_IGNORE_FLOAT_EQUALITY_WARNING_STOP` to properly scope the
/// area where the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_FLOAT_EQUALITY_WARNING_START \
		    _Pragma("GCC diagnostic push")              \
		    _Pragma("GCC diagnostic ignored \"-Wfloat-equal\"")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_FLOAT_EQUALITY_WARNING_START
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_FLOAT_EQUALITY_WARNING_STOP
/// @brief Use to re-enable warnings for checking for floating point equality
/// after having previously used `HYPERION_IGNORE_FLOAT_EQUALITY_WARNING_STOP`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_FLOAT_EQUALITY_WARNING_STOP \
		    _Pragma("GCC diagnostic pop")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_FLOAT_EQUALITY_WARNING_STOP
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_COMMA_MISUSE_WARNING_START
/// @brief Use to temporarily disable warnings for comma operator misuse
/// Make sure to pair with `HYPERION_IGNORE_COMMA_MISUSE_WARNING_STOP` to properly scope the
/// area where the warning is ignored
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_COMMA_MISUSE_WARNING_START \
		    _Pragma("GCC diagnostic push")            \
		    _Pragma("GCC diagnostic ignored \"-Wcomma\"")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_COMMA_MISUSE_WARNING_START
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_COMMA_MISUSE_WARNING_STOP
/// @brief Use to re-enable warnings for comma operator misuse after having previously used
/// `HYPERION_IGNORE_COMMA_MISUSE_WARNING_START`
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"
#if HYPERION_PLATFORM_COMPILER_IS_CLANG
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_COMMA_MISUSE_WARNING_STOP \
		    _Pragma("GCC diagnostic pop")
#else
	// NOLINTNEXTLINE
	#define HYPERION_IGNORE_COMMA_MISUSE_WARNING_STOP
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

/// @def HYPERION_IGNORE_UNSAFE_BUFFER_WARNING_START
/// @brief Use to disable warnings for usage of raw pointers for buffers
/// (clang's '-Wunsafe-buffer-usage`)
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"

/// @def HYPERION_IGNORE_UNSAFE_BUFFER_WARNING_STOP
/// @brief Use to re-enable warnings for usage of raw pointers for buffers
/// (clang's '-Wunsafe-buffer-usage`)
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"

#if HYPERION_PLATFORM_COMPILER_IS_CLANG
    #define HYPERION_IGNORE_UNSAFE_BUFFER_WARNING_START \
	        _Pragma("GCC diagnostic push")              \
	        _Pragma("GCC diagnostic ignored \"-Wunsafe-buffer-usage\"")
    #define HYPERION_IGNORE_UNSAFE_BUFFER_WARNING_STOP \
	        _Pragma("GCC diagnostic pop")
#else
     #define HYPERION_IGNORE_UNSAFE_BUFFER_WARNING_START
    #define HYPERION_IGNORE_UNSAFE_BUFFER_WARNING_STOP
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

// clang-format on

/// @def HYPERION_PLATFORM_PROFILING_ENABLED
/// @brief Indicates whether Tracy profiling is enabled for this build
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"

/// @def HYPERION_PROFILE_FUNCTION
/// @brief Profiles the containing scope with Tracy in builds where Tracy profiling is enabled
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"

/// @def HYPERION_PROFILE_START_FRAME
/// @brief Starts a profiling frame with the given name with Tracy in builds where Tracy
/// profiling is enabled
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"

/// @def HYPERION_PROFILE_END_FRAME
/// @brief Ends the profiling frame with the given name with Tracy in builds where Tracy
/// profiling is enabled
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"

/// @def HYPERION_PROFILE_MARK_FRAME
/// @brief Marks the end of a profiling frame with Tracy in builds where Tracy profiling is
/// enabled
/// @ingroup defines
/// @headerfile "hyperion/platform/def.h"

#ifdef TRACY_ENABLE

HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START
HYPERION_IGNORE_OLD_STYLE_CASTS_WARNING_START
HYPERION_IGNORE_PADDING_WARNING_START
HYPERION_IGNORE_SUGGEST_DESTRUCTOR_OVERRIDE_WARNING_START
    #include <tracy/Tracy.hpp>
HYPERION_IGNORE_SUGGEST_DESTRUCTOR_OVERRIDE_WARNING_STOP
HYPERION_IGNORE_PADDING_WARNING_STOP
HYPERION_IGNORE_OLD_STYLE_CASTS_WARNING_STOP
HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP

    #define HYPERION_PLATFORM_PROFILING_ENABLED /** NOLINT(cppcoreguidelines-macro-usage) **/ true
    #define HYPERION_PROFILE_FUNCTION()         /** NOLINT(cppcoreguidelines-macro-usage) **/     \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START                                        \
        HYPERION_IGNORE_OLD_STYLE_CASTS_WARNING_START                                             \
        /** NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay, hicpp-no-array-decay) **/ \
        ZoneScoped /** NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay,                \
                       hicpp-no-array-decay) **/                                                  \
            HYPERION_IGNORE_OLD_STYLE_CASTS_WARNING_STOP                                          \
                HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP
    #define HYPERION_PROFILE_START_FRAME(name) /** NOLINT(cppcoreguidelines-macro-usage) **/      \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START                                        \
        HYPERION_IGNORE_OLD_STYLE_CASTS_WARNING_START                                             \
        /** NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay, hicpp-no-array-decay) **/ \
        FrameMarkStart(name) /** NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay,      \
                                 hicpp-no-array-decay) **/                                        \
            HYPERION_IGNORE_OLD_STYLE_CASTS_WARNING_STOP                                          \
                HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP
    #define HYPERION_PROFILE_END_FRAME(name) /** NOLINT(cppcoreguidelines-macro-usage) **/        \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START                                        \
        HYPERION_IGNORE_OLD_STYLE_CASTS_WARNING_START                                             \
        /** NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay, hicpp-no-array-decay) **/ \
        FrameMarkEnd(name) /** NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay,        \
                               hicpp-no-array-decay) **/                                          \
            HYPERION_IGNORE_OLD_STYLE_CASTS_WARNING_STOP                                          \
                HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP
    #define HYPERION_PROFILE_MARK_FRAME() /** NOLINT(cppcoreguidelines-macro-usage) **/           \
        HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_START                                        \
        HYPERION_IGNORE_OLD_STYLE_CASTS_WARNING_START                                             \
        /** NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay, hicpp-no-array-decay) **/ \
        FrameMark /** NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay,                 \
                      hicpp-no-array-decay) **/                                                   \
            HYPERION_IGNORE_OLD_STYLE_CASTS_WARNING_STOP                                          \
                HYPERION_IGNORE_RESERVED_IDENTIFIERS_WARNING_STOP
#else
    #define HYPERION_PLATFORM_PROFILING_ENABLED /** NOLINT(cppcoreguidelines-macro-usage) **/ false
    #define HYPERION_PROFILE_FUNCTION()         /** NOLINT(cppcoreguidelines-macro-usage) **/
    #define HYPERION_PROFILE_START_FRAME(name)  /** NOLINT(cppcoreguidelines-macro-usage) **/
    #define HYPERION_PROFILE_END_FRAME(name)    /** NOLINT(cppcoreguidelines-macro-usage) **/
    #define HYPERION_PROFILE_MARK_FRAME()       /** NOLINT(cppcoreguidelines-macro-usage) **/
#endif

HYPERION_IGNORE_UNUSED_MACROS_WARNING_STOP;

#endif // HYPERION_PLATFORM_DEF_H
