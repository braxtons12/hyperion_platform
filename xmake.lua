---@diagnostic disable: undefined-global,undefined-field
set_project("hyperion_platform")
set_version("0.2.4")

set_xmakever("2.8.7")

set_languages("cxx20")

add_rules("mode.debug", "mode.release")
add_moduledirs("xmake")

option("hyperion_enable_tracy", function()
    add_defines("TRACY_ENABLE=1", {public = true})
    set_default(false)
end)

option("hyperion_enable_testing", function()
    add_defines("HYPERION_ENABLE_TESTING=1", {public = true})
    set_default(false)
end)

if has_config("hyperion_enable_tracy") then
    add_requires("tracy", {
        system = false,
        external = true,
        configs = {
            languages = "cxx20"
        }
    })
end

if has_config("hyperion_enable_testing") then
    add_requires("doctest", {
        system = false,
        external = true,
        configs = {
            languages = "cxx20"
        }
    })
end


local hyperion_platform_main_header = {
    "$(projectdir)/include/hyperion/platform.h",
}

local hyperion_platform_headers = {
    "$(projectdir)/include/hyperion/platform/def.h",
    "$(projectdir)/include/hyperion/platform/ignore.h",
    "$(projectdir)/include/hyperion/platform/testing.h",
    "$(projectdir)/include/hyperion/platform/types.h",
}

target("hyperion_platform", function()
    set_kind("headeronly")
    set_languages("cxx20")
    add_includedirs("$(projectdir)/include", { public = true })
    add_headerfiles(hyperion_platform_main_header, { prefixdir = "hyperion", public = true})
    add_headerfiles(hyperion_platform_headers, { prefixdir = "hyperion/platform", public = true })
    set_default(true)
    on_config(function(target)
        import("hyperion_compiler_settings", {alias = "settings"})
        settings.set_compiler_settings(target)
    end)
    add_options("hyperion_enable_tracy", {public = true})
    add_options("hyperion_enable_testing", {public = true})

    if has_package("tracy") then
        add_packages("tracy", {public = true})
    end

    if has_package("doctest") then
        add_packages("doctest", {public = true})
    else
        add_defines("DOCTEST_CONFIG_DISABLE", {public = true})
    end
end)

target("hyperion_platform_main", function()
    set_kind("binary")
    set_languages("cxx20")
    add_includedirs("$(projectdir)/include", { public = true })
    add_headerfiles(hyperion_platform_main_header, { prefixdir = "hyperion" })
    add_headerfiles(hyperion_platform_headers, { prefixdir = "hyperion/platform" })
    add_files("$(projectdir)/src/main.cpp", { prefixdir = "hyperion/platform" })
    add_deps("hyperion_platform")
    set_default(true)
    on_config(function(target)
        import("hyperion_compiler_settings", {alias = "settings"})
        settings.set_compiler_settings(target)
    end)
    add_tests("hyperion_platform_main")
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
