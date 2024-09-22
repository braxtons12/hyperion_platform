Quick Start Guide
*****************

hyperion::platform supports both CMake and XMake, and incorporating it in your project is quick and
easy.

CMake
-----

hyperion::platform is easily incorporated into a raw CMake project with :cmake:`FetchContent` or
other methods like :cmake:`add_subdirectory`\. Example for :cmake:`FetchContent`\:

.. code-block:: cmake
    :caption: CMakeLists.txt
    :linenos:

    # Include FetchContent so we can use it
    include(FetchContent)

    # Declare the dependency on hyperion-utils and make it available for use
    FetchContent_Declare(hyperion_platform
        GIT_REPOSITORY "https://github.com/braxtons12/hyperion_platform"
        GIT_TAG "v0.5.0")
    FetchContent_MakeAvailable(hyperion_platform)

    # For this example, we create an executable target and link hyperion::platform to it
    add_executable(MyExecutable "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
    target_link_libraries(MyExecutable PRIVATE hyperion::platform)

hyperion::platform depends on :cmake:`fast_float` for parsing of literals of the 
:cpp:`f32` and :cpp:`f64` floating point type aliases.
Note that hyperion::platform has an optional feature, for the profiling macros it wraps in
:cpp:`#include <hyperion/platform/def.h>`\. When this feature is enabled, it requires the
optional dependency, `Tracy <https://github.com/wolfpld/tracy>`_\.

By default, this is disabled. You can enable it by setting :cmake:`HYPERION_ENABLE_TRACY` to
:cmake:`ON`\.
If enabled, by default :cmake:`FetchContent` will be used to obtain this optional dependency,
but you can disable this by setting :cmake:`HYPERION_USE_FETCH_CONTENT` to :cmake:`OFF`\,
in which case you will need to make sure the package is findable via CMake's :cmake:`find_package`\.

XMake
-----

XMake is a new(er) Lua-based build system with integrated package management. It is the preferred
way to use Hyperion packages. Example:

.. code-block:: lua
    :caption: xmake.lua
    :linenos:

    set_project("my_project")

    -- add the hyperion_packages git repository as an XMake repository
    add_repositories("hyperion https://github.com/braxtons12/hyperion_packages.git")

    -- add hyperion_platform as a required dependency for the project
    add_requires("hyperion_platform", {
        system = false,
        external = true,
    })
    
    -- For this example, we create an executable target and link hyperion::platform to it
    target("my_executable")
        set_kind("binary")
        add_packages("hyperion_platform")

Note that with XMake, hyperion::platform has the same optional features and dependencies as with the
CMake build system. Third-party dependencies will be pulled from xmake-repo, the package
repository/registry for XMake.

As with CMake, you can enable or disable the Tracy profiling macros (defaults to off) by setting the
option :bash:`hyperion_enable_tracy`\.
