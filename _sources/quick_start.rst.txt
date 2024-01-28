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
        GIT_TAG "origin/main")
    FetchContent_MakeAvailable(hyperion_platform)

    # For this example, we create an executable target and link hyperion::platform to it
    add_executable(MyExecutable "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
    target_link_libraries(MyExecutable PRIVATE hyperion::platform)

Note that hyperion::platform depends on `doctest <https://github.com/doctest/doctestdoctest>`_ for
the testing macros and configuration it wraps in :cpp:`#include <hyperion/platform/testing.h>`\,
and optionally depends on `Tracy <https://github.com/wolfpld/tracy>`_ for the profiling macros it
wraps in :cpp:`#include <hyperion/platform/def.h>`\ (set the option :cmake:`HYPERION_ENABLE_TRACY`
to :cmake:`ON` to enable these). By default, it will use :cmake:`FetchContent` to obtain these
dependencies, but you can disable this by setting :cmake:`HYPERION_USE_FETCH_CONTENT` to
:cmake:`OFF`\, in which case you will need to make sure each package is findable via CMake's
:cmake:`find_package`

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
        -- Don't verify the git commit. This is necessary because hyperion::platform hasn't reached
        -- an official release yet, and thus doesn't have a stable git commit to track.
        -- This allows pulling directly from $HEAD
        verify = false,
    })
    
    -- For this example, we create an executable target and link hyperion::platform to it
    target("my_executable")
        set_kind("binary")
        add_packages("hyperion_platform")
