#include <hyperion/platform.h>
#include <hyperion/platform/types.h>

static_assert(HYPERION_PLATFORM_IS_ARCHITECTURE(HYPERION_PLATFORM_ARCHITECTURE));

using namespace hyperion; // NOLINT(google-build-using-namespace)

[[nodiscard]] auto
main([[maybe_unused]] i32 argc, [[maybe_unused]] const char* const* argv) -> i32 {
    return 0_i32;
}
