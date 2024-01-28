hyperion::platform
******************

hyperion::platform is a foundational library for the Hyperion game engine, providing the core
platform and feature detection macros necessary to implement other parts of the engine and its
related libraries.

It's currently under active development and has not yet hit a stable release point.

hyperion::platform is not dependent on other parts of the engine and can be used freely,
separately from any other Hyperion projects.

See the :doc:`Quick Start Guide <quick_start>` for how to get started using hyperion::platform.

For an overview of each module, see the links in the left sidebar or below.

.. code-block:: cpp
    :caption: Example
    :linenos:

    #include <hyperion/platform.h>
    #include <hyperion/platform/types.h>

    using hyperion::operator""_i32;

    static constexpr auto the_meaning_of_life = 42_i32;

    #if HYPERION_PLATFORM_IS_ARCHITECTURE(HYPERION_PLATFORM_ARCHITECTURE_ARM_V8)
        // do something for ARM
    #elseif HYPERION_PLATFORM_IS_ARCHITECTURE(HYPERION_PLATFORM_ARCHITECTURE_X86_64)
        // do something for x86_64
    #else
        // do something else
    #endif


.. toctree::
    :caption: Getting Started

    quick_start

.. toctree::
    :caption: Quick Reference

    genindex

.. toctree::
    :caption: Platform and Feature Detection Macros
    
    platform

.. toctree::
    :caption: Utility Macros

    def

.. toctree::
    :caption: Core Numeric types

    types

