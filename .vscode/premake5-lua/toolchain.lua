LinkToStandardLibraries = function()
end



local function add_custom_toolset (
    name, 
    toolset_name, 
    prefix, 
    ccompilername, 
    cxxcompilername, 
    archiveutilname, 
    ldlinkername
)
    local toolset = nil

    if toolset_name == "gcc" then
        toolset = premake.tools.gcc
    elseif toolset_name == "clang" then
        toolset = premake.tools.clang
    else
        printf("Toolset %s not supported", toolset_name)
        os.exit(-1)
    end

    local new_toolset                 = {}  
    new_toolset.getcflags             = toolset.getcflags
    new_toolset.getcxxflags           = toolset.getcxxflags
    new_toolset.getcppflags           = toolset.getcppflags
    new_toolset.getdefines            = toolset.getdefines
    new_toolset.getundefines          = toolset.getundefines
    new_toolset.getforceincludes      = toolset.getforceincludes
    new_toolset.getincludedirs        = toolset.getincludedirs
    new_toolset.getldflags            = toolset.getldflags
    new_toolset.getLibraryDirectories = toolset.getLibraryDirectories
    new_toolset.getlinks              = toolset.getlinks
    new_toolset.getmakesettings       = toolset.getmakesettings
    new_toolset.getrunpathdirs        = toolset.getrunpathdirs
    new_toolset.toolset_prefix        = prefix
    new_toolset.shared                = toolset.shared
    new_toolset.shared.compileas      = toolset.shared.compileas

    function new_toolset.gettoolname (cfg, tool)
        if tool == "cc" then
            name = new_toolset.toolset_prefix .. ccompilername
        elseif tool == "cxx" then
            name = new_toolset.toolset_prefix .. cxxcompilername
        elseif tool == "ar" then
            name = new_toolset.toolset_prefix .. archiveutilname
        elseif tool == "ld" then
            name = new_toolset.toolset_prefix .. ldlinkername
        end
        return name
    end  

    premake.tools[name] = new_toolset
end


add_custom_toolset(
    "hipclang", 
    "clang", 
    string.gsub( os.getenv("HIP_BIN_PATH"), "\\", "/" ) .. "/",
    "hipcc",
    "hipcc",
    "llvm-ar",
    "ld.lld --shared"
)



-- -- The reason for this mess: https://groups.google.com/g/llvm-dev/c/WA1vKn9zDtM
-- LinkToStandardLibraries = function()
--     -- Directly Taken from: https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-library-features?view=msvc-170
--     -- Static Debug:
--     -- Libraries: libucrtd.lib libvcruntimed.lib libcmtd.lib libcpmtd.lib
--     -- Macros: _DEBUG, _MT
--     filter { "system:windows", "action:gmake2", "configurations:DebugLib" }
--         buildoptions { "-fms-runtime-lib=static_dbg" }
--         defines { "_MT", "_DEBUG" }
--         linkoptions { "-Wl,/nodefaultlib,kernel32.lib,libcmtd.lib,libcpmtd.lib,libvcruntimed.lib,libucrtd.lib" }

--     -- Static Release: 
--     -- Libraries: libucrt.lib libvcruntime.lib libcmt.lib libcpmt.lib
--     -- Macros: _MT
--     filter { "system:windows", "action:gmake2", "configurations:ReleaseLib" }
--         buildoptions { "-fms-runtime-lib=static" }
--         defines { "_MT" }
--         linkoptions { "-Wl,/nodefaultlib,kernel32.lib,libcmt.lib,libcpmt.lib,libvcruntime.lib,libucrt.lib" }


--     -- Dll Debug: 
--     -- Libraries: ucrtd.lib vcruntimed.lib msvcrtd.lib msvcprtd.lib 
--     -- Macros: _DEBUG, _MT, _DLL
--     filter { "system:windows", "action:gmake2", "configurations:DebugDll" }
--         buildoptions { "-fms-runtime-lib=dll_dbg" }
--         defines { "_MT", "_DEBUG", "_DLL" }
--         linkoptions { "-Wl,/nodefaultlib,kernel32.lib,msvcrtd.lib,msvcprtd.lib,vcruntimed.lib,ucrtd.lib" }

--     -- Dll Release: 
--     -- Libraries: ucrt.lib vcruntime.lib msvcrt.lib msvcprt.lib
--     -- Macros: _MT, _DLL
--     filter { "system:windows", "action:gmake2", "configurations:ReleaseDll" }
--         buildoptions { "-fms-runtime-lib=dll" }
--         defines { "_MT", "_DLL" }
--         linkoptions { "-Wl,/nodefaultlib,kernel32.lib,msvcrt.lib,msvcprt.lib,vcruntime.lib,ucrt.lib" }

--     filter {}
-- end


-- Primarily for debugging
-- filter { "toolset:clang", "system:windows" }
--     makesettings {
--         "CC  = " .. '"' .. "clang.exe"   .. '"' .. " --verbose",
--         -- "CXX = " .. '"' .. "clang++.exe" .. '"' .. " --verbose -ferror-limit=0 -fuse-ld=" .. '"' .. "C:/CTools/msys64/clang64/bin/lld-link" .. '"',
--         "CXX = " .. '"' .. "clang++.exe" .. '"' .. " --verbose -ferror-limit=0 -fuse-ld=" .. '"' .. "C:/CTools/msys2/clang64/bin/ld.lld" .. '"',
--         "LD  = " .. '"' .. "ld.lld.exe"  .. '"' .. " --verbose",
--         "AR  = " .. '"' .. "llvm-ar.exe" .. '"' .. " v"
--     }
-- filter {}
-- filter { "toolset:clang", "system:not windows" }
--     makesettings {
--         "CC  = " .. '"' .. "clang"   .. '"' .. " --verbose",
--         "CXX = " .. '"' .. "clang++" .. '"' .. " --verbose -ferror-limit=0 -fuse-ld=" .. '"' .. "/usr/bin/ld.lld" .. '"',
--         "LD  = " .. '"' .. "ld.lld"  .. '"' .. " --verbose",
--         "AR  = " .. '"' .. "llvm-ar" .. '"' .. " v"
--     }