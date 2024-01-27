---@diagnostic disable: undefined-global,undefined-field
set_project("hyperion_platform")
set_version("0.1.0")

set_xmakever("2.8.2")

option("hyperion_enable_tracy", function()
    add_defines("TRACY_ENABLE")
    set_default(false)
    after_check(function(opt)
        if opt:enabled() then
            add_requires("tracy", {
                system = false,
                external = true,
                configs = {
                    languages = "cxx20"
                }
            })
        end
    end)
end)

local hyperion_platform_main_header = {
    "$(projectdir)/include/hyperion/platform.h",
}

local hyperion_platform_headers = {
    "$(projectdir)/include/hyperion/platform/def.h",
    "$(projectdir)/include/hyperion/platform/types.h",
}

target("hyperion_platform", function()
    set_kind("headeronly")
    set_languages("cxx20")
    add_includedirs("$(projectdir)/include", { public = true })
    add_headerfiles(hyperion_platform_main_header, { prefixdir = "hyperion" })
    add_headerfiles(hyperion_platform_headers, { prefixdir = "hyperion/platform" })
    set_default(true)
    after_build(function(_)
        local cmd = "xmake check clang.tidy"
        os.run(cmd)
    end)
    on_config(function(target)
        import("xmake.hyperion_compiler_settings", {alias = "settings"})
        settings.set_compiler_settings(target)
    end)
    add_options("hyperion_enable_tracy")

    if has_package("tracy") then
        add_deps("tracy")
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
    after_build(function(_)
        local cmd = "xmake check clang.tidy"
        os.run(cmd)
    end)
    on_config(function(target)
        import("xmake.hyperion_compiler_settings", {alias = "settings"})
        settings.set_compiler_settings(target)
    end)
end)

target("hyperion_platform_docs", function()
    set_kind("phony")
    set_default(false)
    on_build(function(_)
        local old_dir = os.curdir()
        os.cd(os.scriptdir() .. "/docs")
        os.exec("sphinx-build -M html . _build")
        os.cd(old_dir)
    end)
end)