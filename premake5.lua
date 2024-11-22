-- Makes a path relative to the folder containing this script file.
ROOT_PATH = function(path)
    return string.format("%s/%s", _MAIN_SCRIPT_DIR, path)
end


GetLibraryType = function()
    defaultplatform "Static"
    filter "platforms:Static"
        kind "StaticLib"
        staticruntime "on"
    filter "platforms:Shared"
        kind "SharedLib"
        staticruntime "off"
end


PROJECT_DIR     = ROOT_PATH "projects"
DEPENDENCY_DIR  = ROOT_PATH "dependencies"
BIN_DIR         = ROOT_PATH "build/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
OBJ_DIR         = ROOT_PATH "build/obj/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
WORKSPACE_NAME  = "example-awc2-program"
START_PROJECT   = "example-mdk"


include "workspace.lua"
include "projects.lua"
for _, path in ipairs(PROJECTS) do
    include(path .. "/premake5.lua")
end


-- Add Option to compile for a certain architecture, may not necessarily match the machine architecture
newoption {
    trigger     = "arch",
    description = "Manually Specify the architecture of the project",
    value       = "x86_64",
    allowed = {
        { "universal", "Universal Binaries Supported by ios & macOS " },
        { "i386",                           "Alias for x86" },
        { "x86",                                         "" },
        { "amd64",                       "Alias for x86_64" },
        { "x86_64",                                      "" },
        { "ARM",                                         "" },
        { "ARM64",                                       "" },
        { "armv5",   "Only supported in VSAndroid projects" },
        { "armv7",   "Only supported in VSAndroid projects" },
        { "aarch64", "Only supported in VSAndroid projects" },
        { "mips",    "Only supported in VSAndroid projects" },
        { "mips64",  "Only supported in VSAndroid projects" },
    },
    default = "x86_64"
}


-- Rebuild Project Solutions' Function --
newaction {
    trigger     = "CleanProjectConfigs",
    description = "Remove all Project Solutions, Makefiles, Ninja build files, etc...",
    execute     = function ()
        local build_extensions = { "/Makefile", "/**.sln", "/**.vcxproj" }
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
        os.remove("build/**") 
        os.rmdir("build/**")
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

