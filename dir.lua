-- Build Configuration
BUILD_BINARY_DIRECTORY_GENERIC = "/build/bin/%{cfg.buildcfg}_%{cfg.platform}_%{prj.name}"
BUILD_OBJECT_DIRECTORY_GENERIC = "/build/obj/%{cfg.buildcfg}_%{cfg.platform}_%{prj.name}"
BUILD_BINARY_DIRECTORY         = "/build/bin/%{cfg.buildcfg}_%{cfg.platform}"


-- The Idea behind this is that the debug configuration on 'workspace.lua'
-- is not enough for lldb on windows. I need to manually tell the linker to generate pdb files
-- (makes sense all in all, gcc doesn't need pdb files to debug programs compiled with -g)
-- Needs Testing with gdb on linux / WSL
SetupLinkingFlagsForDebugInfo = function()
    local pdbpath = '"' .. _MAIN_SCRIPT_DIR .. BUILD_BINARY_DIRECTORY_GENERIC ..  "/%{prj.name}.pdb" .. '"'


    filter { "system:windows", "configurations:Debug* or configurations:Production*", "action:gmake" }
        linkoptions( "-g -Wl,--pdb=-Wl,--pdb=" .. pdbpath )
    filter {}
    -- Incomplete and untested, when working with vs2022 figure it out
    filter { "system:windows", "configurations:Debug* or configurations:Production*", "action:vs2022" }
        symbolspath( pdbpath )
    filter {}
end


SetupBuildDirectoriesForLibrary = function()
    filter "configurations:*Lib"
        kind "StaticLib"
        targetdir (_MAIN_SCRIPT_DIR .. BUILD_BINARY_DIRECTORY_GENERIC)
        objdir    (_MAIN_SCRIPT_DIR .. BUILD_OBJECT_DIRECTORY_GENERIC)
    filter {}

    filter "configurations:*Dll"
        kind "SharedLib"
        targetdir (_MAIN_SCRIPT_DIR .. BUILD_BINARY_DIRECTORY_GENERIC)
        objdir    (_MAIN_SCRIPT_DIR .. BUILD_OBJECT_DIRECTORY_GENERIC)
    filter {}
    SetupLinkingFlagsForDebugInfo()
end


SetupBuildDirectoriesForExecutable = function()
    filter "configurations:*Lib"
        targetdir (_MAIN_SCRIPT_DIR .. BUILD_BINARY_DIRECTORY_GENERIC)
        objdir    (_MAIN_SCRIPT_DIR .. BUILD_OBJECT_DIRECTORY_GENERIC)
    filter {}

    filter "configurations:*Dll"
        targetdir (_MAIN_SCRIPT_DIR .. BUILD_BINARY_DIRECTORY_GENERIC)
        objdir    (_MAIN_SCRIPT_DIR .. BUILD_OBJECT_DIRECTORY_GENERIC)
    filter {}
    SetupLinkingFlagsForDebugInfo()
end


IncludeProjectHeaders = function(ProjectName)
    includedirs { _MAIN_SCRIPT_DIR .. "/projects/" .. ProjectName .. "/include" }
end


LinkProjectLibrary = function(ProjectName)
    links { ProjectName }
end


LinkUtil2Library = function()
    LinkProjectLibrary("util2")
    filter { "configurations:*Lib" }
        defines { "UTIL2_STATIC_DEFINE" }
    filter {}
end