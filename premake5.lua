require("export-compile-commands")


-- Makes a path relative to the folder containing this script file.
ROOT_PATH = function(path)
    return string.format("%s/%s", _MAIN_SCRIPT_DIR, path)
end


PROJECT_DIR     = ROOT_PATH("projects")
DEPENDENCY_DIR  = ROOT_PATH("dependencies")
WORKSPACE_NAME  = "example-awc2-program"
START_PROJECT   = "program-test"




SpecifyGlobalProjectCXXVersion = function()
    language "C++"
    cppdialect "C++17"
    filter "toolset:gcc"
        cppdialect "gnu++17"
    filter ""
    filter {}
end


SetupBuildDirectoriesForLibrary = function()
    filter "configurations:*Lib"
        kind "StaticLib"
        staticruntime "on"
        targetdir (_MAIN_SCRIPT_DIR .. "/build/bin/%{cfg.buildcfg}_%{cfg.platform}_%{prj.name}")
        objdir    (_MAIN_SCRIPT_DIR .. "/build/obj/%{cfg.buildcfg}_%{cfg.platform}_%{prj.name}")
    filter ""

    filter "configurations:*Dll"
        kind "SharedLib"
        staticruntime "off"
        targetdir (_MAIN_SCRIPT_DIR .. "/build/bin/%{cfg.buildcfg}_%{cfg.platform}_%{prj.name}")
        objdir    (_MAIN_SCRIPT_DIR .. "/build/obj/%{cfg.buildcfg}_%{cfg.platform}_%{prj.name}")
    filter ""
    filter {}
end


SetupBuildDirectoriesForExecutable = function()
    filter "configurations:*Lib"
        staticruntime "on"
        targetdir (_MAIN_SCRIPT_DIR .. "/build/bin/%{cfg.buildcfg}_%{cfg.platform}_%{prj.name}")
        objdir    (_MAIN_SCRIPT_DIR .. "/build/obj/%{cfg.buildcfg}_%{cfg.platform}_%{prj.name}")
    filter ""

    filter "configurations:*Dll"
        staticruntime "off"
        targetdir (_MAIN_SCRIPT_DIR .. "/build/bin/%{cfg.buildcfg}_%{cfg.platform}_%{prj.name}")
        objdir    (_MAIN_SCRIPT_DIR .. "/build/obj/%{cfg.buildcfg}_%{cfg.platform}_%{prj.name}")
    filter ""
    filter {}
end


IncludeGLFWDirectory = function()
    includedirs { DEPENDENCY_DIR .. "/GLFW/include" }
end


LinkGLFWLibrary = function()
    filter "system:windows"
        libdirs { DEPENDENCY_DIR .. "/GLFW/windows/%{cfg.architecture}/lib-vc2022" }
    filter ""
    filter { "system:windows", "configurations:*Lib" }
        links { "glfw3_mt" }
    filter { "system:windows", "configurations:*Dll" }
        links { "glfw3dll" }
        defines { "GLFW_DLL" }
    filter {}
    filter "system:linux" -- requires the following packages [debian]: apt-get install libglfw3 libglfw3-dev libgl-dev 
        links { "glfw" }
    filter ""
end


-- try to use these generic functions to include & link every project, right now the only ones popping up on program is glfw (also for awc2 ...)
IncludeProjectHeaders = function(ProjectName)
    includedirs { ROOT_PATH("projects/" .. ProjectName .. "/include") }
end

LinkProjectLibrary = function(ProjectName)
    filter { "configurations:*Lib" }
        links { ProjectName }
    filter { "configurations:*Dll" }
        links { ProjectName }
    filter {}
end


LinkUtilLibrary = function()
    LinkProjectLibrary("util")
    filter { "configurations:*Lib" }
        defines { "UTIL_STATIC_DEFINE" }
    filter {}
end

LinkGLBindingLibraries = function()
    LinkProjectLibrary("glbinding")
    filter { "configurations:*Lib" }
        defines { "GLBINDING_STATIC_DEFINE" }
    filter {}
    LinkProjectLibrary("glbinding-aux")
    filter { "configurations:*Lib" }
        defines { "GLBINDING_AUX_STATIC_DEFINE" }
    filter {}
end




include "workspace.lua"
include "projects.lua"
for _, path in ipairs(PROJECTS) do
    include(path .. "/premake5.lua")
end


-- Add Option to compile for a certain architecture, may not necessarily match the machine architecture
-- [NOTE]: Deprecated and not used anymore
-- newoption {
--     trigger     = "arch",
--     description = "Manually Specify the architecture of the project",
--     value       = "x86_64",
--     allowed = {
--         { "universal", "Universal Binaries Supported by ios & macOS " },
--         { "i386",                           "Alias for x86" },
--         { "x86",                                         "" },
--         { "amd64",                       "Alias for x86_64" },
--         { "x86_64",                                      "" },
--         { "ARM",                                         "" },
--         { "ARM64",                                       "" },
--         { "armv5",   "Only supported in VSAndroid projects" },
--         { "armv7",   "Only supported in VSAndroid projects" },
--         { "aarch64", "Only supported in VSAndroid projects" },
--         { "mips",    "Only supported in VSAndroid projects" },
--         { "mips64",  "Only supported in VSAndroid projects" },
--     },
--     default = "x86_64"
-- }


-- Rebuild Project Solutions' Function --
newaction {
    trigger     = "CleanProjectConfigs",
    description = "Remove all Project Solutions, Makefiles, Ninja build files, etc...",
    execute     = function ()
        local build_extensions = { "/Makefile", "/**.sln", "/**.vcxproj", "/**.vcxproj.filters", "/**.vcxproj.user", "/**.ninja", "/.ninja_deps", "/.ninja_log", "/.ninja_lock" }
        local ok, err
        table.insert(PROJECTS, ".")
        for _, path in ipairs(PROJECTS) do
            local flag = true
            printf("%s", path)

            for __, ext in ipairs(build_extensions) do
                ok, err = os.remove{ path .. ext }
                if ok then
                    printf("%-30s: %s", "Removed Project Config Files", path .. ext)
                else
                    printf("%-30s: %s", "Got Error", err)
                end
            end
            printf("----------------------------------------")
        end
        table.remove(PROJECTS)
        print("Done")
    end
}


-- Wipe Function --
newaction {
    trigger     = "CleanAllBuild",
    description = "Delete All Object Files & Executables created during the build process",
    execute     = function ()
        local dirs_to_delete = { "./build", "./.vs" }
        local ok, err
        printf("----------------------------------------")
        for _, path in ipairs(dirs_to_delete) do
            ok, err = os.rmdir(path)
            if ok then
                printf("%-30s: %s", "Removed Directory", path)
            else
                printf("%-30s: %s", "Got Error", err)
            end
        end
        printf("----------------------------------------")
        print("Done")
    end
}


-- Delete whatever was generated using export-compile-commands
newaction {
    trigger     = "CleanClangd",
    description = "Delete the compile_commands/* Directory",
    execute     = function ()
        local ok, err, path
        printf("----------------------------------------")
        path = "./compile_commands"
        ok, err = os.rmdir(path)
        if ok then
            printf("%-30s: %s", "Removed Directory", path)
        else
            printf("%-30s: %s", "Got Error", err)
        end
        printf("----------------------------------------")
        print("Done")
    end
}


-- Clean Specific System--Architecture Build Function [TODO] --
newaction {
    trigger     = "CleanBuild[TODO]",
    description = "Delete All Debug & Release files generated for a specific system-architecture (e.g windows-x86_64)",
    execute     = function ()
        os.remove("build/bin/Debug-") 
        os.rmdir("build/**")
        print("Done")
    end
}

