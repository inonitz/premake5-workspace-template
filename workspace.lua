workspace (WORKSPACE_NAME)
    startproject(START_PROJECT)
    architecture "x86_64" -- can be overriden using --arch='x' flag
    

    -- 'system' can be overriden using --os='x' flag
    filter { "system:windows", "action:vs2022" }
        toolset "msc-llvm-vs2022"
        llvmdir     = os.getenv("LLVMInstallDir")
        llvmversion = os.getenv("LLVMToolsVersion")
        flags { "MultiProcessorCompile" } -- NOTE: no equivalent in linux, must use makefiles and -j$NPROC
    filter {}
    filter { "system:windows", "action:gmake2" } -- Should Still Be Relevant In the future: https://stackoverflow.com/questions/29504627/adjusting-g-location-with-premake
        toolset "clang"
        llvmdir     = os.getenv("LLVMInstallDir")
        llvmversion = os.getenv("LLVMToolsVersion")
        -- flags { "LinkTimeOptimization" } -- easy fix to switch from 'ar' to 'llvm-ar' 
        makesettings {
            "CC = " .. llvmdir .. "/bin/clang.exe --verbose",
            "CXX = " .. llvmdir .. "/bin/clang++.exe -ferror-limit=0 --verbose -fuse-ld=lld-link.exe",
            "LD = " .. llvmdir .. "/bin/ld.lld.exe --verbose",
            "AR = " .. llvmdir .. "/bin/llvm-ar.exe --verbose"
        }
    filter {}
    filter "system:linux"
        toolset "clang"
    filter {}


    configurations {
        "DebugLib",
        "ReleaseLib",
        "DebugDll",
        "ReleaseDll"
    }
    platforms {
        "x86",
        "amd64",
        "ARM",
        "ARM64"
    }


    -- We give priority to Platform over the 'arch' flag
    filter "platforms:x86"
        architecture "x86"
    filter "platforms:amd64"
        architecture "x86_64"
    filter "platforms:ARM"
        architecture "ARM"
    filter "platforms:ARM64"
        architecture "ARM64"
    filter {}

    filter "configurations:Debug*"
        defines { "DEBUG" }
        runtime  "Debug"
        symbols  "on"
        optimize "off"
    filter "configurations:Release*"
        defines { "NDEBUG" }
        runtime  "Release"
        symbols  "off"
        optimize "on"
    filter {}

    filter "architecture:x86"
        defines { "__x86__" }
    filter "architecture:x86_64"
        defines { "__x86_64__" }
    filter "system:linux"
        defines { "__linux__" }
    filter { "system:windows", "architecture:x86" }
        defines { "_WIN32" }
    filter { "system:windows", "architecture:x86_64" }
        defines { "_WIN64" }
    filter {}