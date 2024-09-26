cmake_minimum_required(VERSION 3.20)
project(hyperion_platform LANGUAGES CXX VERSION 0.5.3)

include(CTest)

option(HYPERION_ENABLE_TRACY "Enables Profiling with Tracy" OFF)
option(HYPERION_USE_FETCH_CONTENT "Enables FetchContent usage for getting dependencies" ON)

if(HYPERION_USE_FETCH_CONTENT)
    include(FetchContent)
endif()

if(${HYPERION_ENABLE_TRACY})
    if(HYPERION_USE_FETCH_CONTENT)
        FetchContent_Declare(Tracy
            GIT_REPOSITORY "https://github.com/wolfpld/tracy"
            GIT_TAG "v0.11.1"
            SYSTEM
        )

        FetchContent_MakeAvailable(Tracy)
    else()
        find_package(Tracy REQUIRED)
    endif()
    SET(TRACY_LINK_TARGET Tracy::TracyClient)
endif()

if(HYPERION_USE_FETCH_CONTENT)
    FetchContent_Declare(fast_float
        GIT_REPOSITORY "https://github.com/fastfloat/fast_float"
        GIT_TAG "v6.1.6"
        GIT_SHALLOW TRUE
        SYSTEM
    )
    FetchContent_MakeAvailable(fast_float)

    FetchContent_Declare(boost_ut
        GIT_REPOSITORY "https://github.com/boost-ext/ut"
        GIT_TAG "v2.1.0"
        SYSTEM
    )
    FetchContent_MakeAvailable(boost_ut)
else()
    find_package(ut REQUIRED)
    find_package(fast_float REQUIRED)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/hyperion_compiler_settings.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/hyperion_enable_warnings.cmake)

set(HYPERION_PLATFORM_INCLUDE_DIRECTORIES
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
set(HYPERION_PLATFORM_INCLUDE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/include/hyperion"
)
set(HYPERION_PLATFORM_HEADERS
    "${HYPERION_PLATFORM_INCLUDE_PATH}/platform.h"
    "${HYPERION_PLATFORM_INCLUDE_PATH}/platform/def.h"
    "${HYPERION_PLATFORM_INCLUDE_PATH}/platform/ignore.h"
    "${HYPERION_PLATFORM_INCLUDE_PATH}/platform/types.h"
    "${HYPERION_PLATFORM_INCLUDE_PATH}/platform/compare.h"
)

add_library(hyperion_platform INTERFACE)
add_library(hyperion::platform ALIAS hyperion_platform)
target_include_directories(
    hyperion_platform
    INTERFACE
    ${HYPERION_PLATFORM_INCLUDE_DIRECTORIES}
)
target_sources(
    hyperion_platform
    INTERFACE
    ${HYPERION_PLATFORM_HEADERS}
)
target_link_libraries(
    hyperion_platform
    INTERFACE
    ${TRACY_LINK_TARGET}
    fast_float
)

hyperion_compile_settings(hyperion_platform)
hyperion_enable_warnings(hyperion_platform)

if(${HYPERION_ENABLE_TRACY})
    target_compile_definitions(
        hyperion_platform
        INTERFACE
        TRACY_ENABLE=1
    )
endif()

add_executable(hyperion_platform_main ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(hyperion_platform_main
    PRIVATE
    hyperion::platform
)

hyperion_compile_settings(hyperion_platform_main)
hyperion_enable_warnings(hyperion_platform_main)

add_test(NAME hyperion_platform_main
         COMMAND hyperion_platform_main)

 set(HYPERION_PLATFORM_TEST_SOURCES
     ${CMAKE_CURRENT_SOURCE_DIR}/src/test_main.cpp
     #${CMAKE_CURRENT_SOURCE_DIR}/src/test/compare.cpp
 )
 add_executable(hyperion_platform_tests ${HYPERION_PLATFORM_TEST_SOURCES})
target_link_libraries(hyperion_platform_tests
    PRIVATE
    hyperion::platform
    Boost::ut
)
target_compile_definitions(
    hyperion_platform_tests
    PUBLIC
    BOOST_UT_DISABLE_MODULE=1
)

hyperion_compile_settings(hyperion_platform_tests)
hyperion_enable_warnings(hyperion_platform_tests)

add_test(NAME hyperion_platform_tests
         COMMAND hyperion_platform_tests)

set(HYPERION_PLATFORM_DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/docs/_build/html")
set(HYPERION_PLATFORM_DOXYGEN_HTML "${HYPERION_PLATFORM_DOXYGEN_OUTPUT_DIR}/index.html")

set(HYPERION_PLATFORM_DOCS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/docs")
set(HYPERION_PLATFORM_DOCS_FILES
    "${HYPERION_PLATFORM_DOCS_DIR}/index.rst"
    "${HYPERION_PLATFORM_DOCS_DIR}/platform.rst"
    "${HYPERION_PLATFORM_DOCS_DIR}/def.rst"
    "${HYPERION_PLATFORM_DOCS_DIR}/quick_start.rst"
    "${HYPERION_PLATFORM_DOCS_DIR}/types.rst"
)

add_custom_command(
    OUTPUT ${HYPERION_PLATFORM_DOXYGEN_HTML}
    DEPENDS ${HYPERION_PLATFORM_HEADERS} ${DOCS_FILES}
    COMMAND sphix-build -M html . _build
    WORKING_DIRECTORY ${HYPERION_PLATFORM_DOCS_DIR}
    MAIN_DEPENDENCY
    COMMENT "Generating documentation with doxygen, sphinx, and breathe"
    VERBATIM
)

add_custom_target(hyperion_platform_doxygen DEPENDS ${DOXYGEN_HTML})

include(GNUInstallDirs)
install(DIRECTORY ${HYPERION_PLATFORM_DOXYGEN_OUTPUT_DIR} DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(DIRECTORY include/ DESTINATION include)
install(TARGETS hyperion_platform DESTINATION lib)
