# hyperion::platform

hyperion::platform is a foundational library for the Hyperion game engine, providing the core
platform/feature detection and utility macros necessary to implement other parts of the engine and
its related libraries, as well as other core library utilities.

It's currently under active development and has not yet hit a stable release point.

hyperion::platform is not dependent on other parts of the engine and can be used freely,
separately from any other Hyperion projects.

### Quick Start

See the [Quick Start Guide](https://braxtons12.github.io/hyperion_platform/quick_start.html)
for how to get started using hyperion::platform.

### Documentation

You can also find the rest of the project documentation [here](https://braxtons12.github.io/hyperion_platform/)

### Example

After following the Quick Start Guide, you'll be able to use hyperion::platform in your project.
A basic example of what you can do with hyperion::platform is below:

```cpp
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
```

### Contributing

Feel free to submit issues, pull requests, etc.!<br>
When contributing code, please follow the general style expectations of Hyperion projects:
- Follow the project `.clang-format` (except in judicious cases of templates or requires clauses
        ruining formatting),
- Use trailing returns types,
- Use assign-init (e.g. `auto var = value;`) instead of direct-init (e.g. `auto var{value}`),
- By default, prefer simplicity and correctness over performance

### License

hyperion::platform uses the MIT license.

