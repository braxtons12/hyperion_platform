hyperion::platform
******************

hyperion::platform is a foundational C++ library for cross-platform projects, providing several common
features to detect, work around, or abstract away the differences between various platforms and
toolchains, such as operating system detection macros, CPU architecture detection macros, compiler
and library feature detection macros, and bounds-checked numeric literals.

hyperion::platform is a core library of the Hyperion game engine project.

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
    #elif HYPERION_PLATFORM_IS_ARCHITECTURE(HYPERION_PLATFORM_ARCHITECTURE_X86_64)
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

.. toctree::
    :caption: Core Library Utilities

    utility
