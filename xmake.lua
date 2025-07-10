---@diagnostic disable: undefined-global,undefined-field
set_project("hyperion_platform")
set_version("0.5.3")

set_xmakever("3.0.0")

set_languages("cxx20")

add_rules("mode.debug", "mode.check", "mode.coverage", "mode.release")
add_moduledirs("xmake")

option("hyperion_enable_tracy", function()
    add_defines("TRACY_ENABLE=1", {public = true})
    set_default(false)
end)

if has_config("hyperion_enable_tracy") then
    add_requires("tracy", {
        system = false,
        configs = {
            languages = "cxx20"
        }
    })
end

add_requires("fast_float", {
    system = false,
    configs = {
        languages = "cxx20",
    },
})

add_requires("boost_ut", {
    system = false,
    configs = {
        modules = false,
        languages = "cxx20",
    }
})

local hyperion_platform_main_header = {
    "$(projectdir)/include/hyperion/platform.h",
}

local hyperion_platform_headers = {
    "$(projectdir)/include/hyperion/platform/def.h",
    "$(projectdir)/include/hyperion/platform/ignore.h",
    "$(projectdir)/include/hyperion/platform/types.h",
    "$(projectdir)/include/hyperion/platform/compare.h",
}

target("hyperion_platform", function()
    set_kind("headeronly")
    set_languages("cxx20")
    set_default(true)

    add_includedirs("$(projectdir)/include", { public = true })
    add_headerfiles(hyperion_platform_main_header, { prefixdir = "hyperion", public = true})
    add_headerfiles(hyperion_platform_headers, { prefixdir = "hyperion/platform", public = true })

    on_config(function(target)
        import("hyperion_compiler_settings", {alias = "settings"})
        settings.set_compiler_settings(target)
    end)

    add_packages("fast_float", {public = true})

    add_options("hyperion_enable_tracy", {public = true})
    if has_package("tracy") then
        add_packages("tracy", {public = true})
    end
end)

target("hyperion_platform_main", function()
    set_kind("binary")
    set_languages("cxx20")
    set_default(true)

    add_files("$(projectdir)/src/main.cpp", { prefixdir = "hyperion/platform" })

    add_deps("hyperion_platform")

    on_config(function(target)
        import("hyperion_compiler_settings", {alias = "settings"})
        settings.set_compiler_settings(target)
    end)
    add_tests("hyperion_platform_main")
end)

local hyperion_platform_test_sources = {
    "$(projectdir)/src/test_main.cpp",
}
target("hyperion_platform_tests", function()
    set_kind("binary")
    set_languages("cxx20")
    set_default(true)

    add_files(hyperion_platform_test_sources)
    add_defines("BOOST_UT_DISABLE_MODULE=1")

    add_deps("hyperion_platform")
    add_packages("boost_ut")

    on_config(function(target)
        import("hyperion_compiler_settings", { alias = "settings" })
        settings.set_compiler_settings(target)
    end)

    add_tests("hyperion_platform_tests")
end)

target("hyperion_platform_docs", function()
    set_kind("phony")
    set_default(false)
    on_build(function(_)
        local old_dir = os.curdir()
        os.cd("$(projectdir)/docs")
        os.exec("sphinx-build -M html . _build")
        os.cd(old_dir)
    end)
end)
