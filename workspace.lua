workspace (WORKSPACE_NAME)
    startproject(START_PROJECT)
    -- 'arch'    can be overriden using --arch='x' flag
    -- 'system'  can be overriden using --os='x'   flag
    -- 'toolset' can be overriden using --cc='x'   flag (mostly...)
    filter { "system:windows", "action:vs2022" }
        toolset "msc-llvm-vs2022"
        if llvmdir and llvmversion then
            llvmdir     = os.getenv("LLVMInstallDir")
            llvmversion = os.getenv("LLVMToolsVersion")
        end
        flags { "MultiProcessorCompile" } -- NOTE: no equivalent in linux, must use makefiles and -j$NPROC
    filter {}
    filter { "system:windows", "action:gmake" } -- This article should help in the future: https://stackoverflow.com/questions/29504627/adjusting-g-location-with-premake
        toolset "clang"
        if llvmdir and llvmversion then
            llvmdir     = os.getenv("LLVMInstallDir")
            llvmversion = os.getenv("LLVMToolsVersion")
            -- flags { "LinkTimeOptimization" } -- easy fix to switch from 'ar' to 'llvm-ar' 
            makesettings {
                "CC = "  .. '"' .. llvmdir .. "/bin/clang.exe"   .. '"' .. " --verbose",
                "CXX = " .. '"' .. llvmdir .. "/bin/clang++.exe" .. '"' .. " --verbose -ferror-limit=0 -fuse-ld=lld-link.exe",
                "LD = "  .. '"' .. llvmdir .. "/bin/ld.lld.exe"  .. '"' .. " --verbose",
                "AR = "  .. '"' .. llvmdir .. "/bin/llvm-ar.exe" .. '"' .. " v"
            }
            -- makesettings {
            --     "CC = "  .. '"' .. llvmdir .. "/bin/clang.exe"   .. '"' .. "",
            --     "CXX = " .. '"' .. llvmdir .. "/bin/clang++.exe" .. '"' .. " -ferror-limit=0 -fuse-ld=lld-link.exe",
            --     "LD = "  .. '"' .. llvmdir .. "/bin/ld.lld.exe"  .. '"' .. "",
            --     "AR = "  .. '"' .. llvmdir .. "/bin/llvm-ar.exe" .. '"' .. " v"
            -- }
        end
    filter {}
    -- #1 Link: https://askubuntu.com/questions/1508260/how-do-i-install-clang-18-on-ubuntu
    -- #2 Link: https://unix.stackexchange.com/questions/596226/how-to-change-clang-10-llvm-10-etc-to-clang-llvm-etc
    -- #3 I expect clang to be installed and symlinked on your machine, basically.
    -- #4 Why is everything easier on linux?!?
    filter "system:linux"
        toolset "clang"
        debugformat "Dwarf"
        -- if llvmdir then
        --     llvmdir = os.getenv("LLVMInstallDir")
        -- end
        -- makesettings {
        --     "CC = "  .. '"' .. llvmdir .. "/clang"   .. '"' .. " --verbose",
        --     "CXX = " .. '"' .. llvmdir .. "/clang++" .. '"' .. " --verbose -ferror-limit=0",
        --     "LD = "  .. '"' .. llvmdir .. "/ld.lld"  .. '"' .. " --verbose",
        --     "AR = "  .. '"' .. llvmdir .. "/llvm-ar-19" .. '"' .. " v"
        -- }
    filter {}


    configurations {
        "DebugLib",
        "DebugDll",
        "ReleaseLib",
        "ReleaseDll",
        "ProductionLib",
        "ProductionDll",
    }
    platforms {
        "x86",
        "amd64",
        "ARM",
        "ARM64"
    }

    filter "platforms:x86"
        architecture "x86"
    filter "platforms:amd64"
        architecture "x86_64"
    filter "platforms:ARM"
        architecture "ARM"
    filter "platforms:ARM64"
        architecture "ARM64"
    filter {}
    -- This Might be problematic for standard headers. Triple Check later
    -- filter "architecture:x86"
    --     defines { "__x86__" }
    -- filter "architecture:x86_64"
    --     defines { "__x86_64__" }
    -- filter "system:linux"
    --     defines { "__linux__" }
    -- filter { "system:windows", "architecture:x86" }
    --     defines { "_WIN32" }
    -- filter { "system:windows", "architecture:x86_64" }
    --     defines { "_WIN64" }
    -- filter {}


    filter { "toolset:not gcc", "files:**.c" }
        cdialect "C11"
    filter { "toolset:not gcc", "files:**.h" }
        cdialect "C11"
    filter { "toolset:not gcc", "files:**.cpp" }
        cppdialect "C++17"
    filter { "toolset:not gcc", "files:**.hpp" }
        cppdialect "C++17"
    filter {}
    filter { "toolset:gcc", "files:**.c" }
        cdialect "gnu11"
    filter { "toolset:gcc", "files:**.h" }
        cdialect "gnu11"
    filter { "toolset:gcc", "files:**.cpp" }
        cppdialect "gnu++17"
    filter { "toolset:gcc", "files:**.hpp" }
        cppdialect "gnu++17"
    filter {}


    -- Compiler Flags
    -- Debug Configuration Across Multiple Platforms
    filter { "configurations:Debug*" }
        defines { "DEBUG" }
        runtime  "Debug"
        optimize "Off"
        symbols  "On"

    filter { "configurations:Debug*", "toolset:gcc" }
        buildoptions { 
            "-g",
            "-ggdb"
        }

    filter { "configurations:Debug*", "toolset:clang" }
        buildoptions { 
            "-g", 
            "-fno-limit-debug-info", 
            "-fstandalone-debug",
            "-gcolumn-info", 
            "-glldb"
        }
    filter {}


    -- Production Configuration Across Multiple Platforms
    filter { "configurations:Production*" }
        defines { "DEBUG" }
        runtime  "Release"
        optimize "On"
        symbols  "On"

    filter { "configurations:Production*", "toolset:gcc" }
        buildoptions { 
            "-g",
            "-ggdb"
        }

    filter { "configurations:Production*", "toolset:clang" }
        buildoptions { 
            "-g", 
            "-fno-limit-debug-info", 
            "-fstandalone-debug",
            "-gcolumn-info", 
            "-glldb"
        }
    filter {}


    -- Specify Address Sanitizer for Debug/Production
    filter { "configurations:Debug* or configurations:Production*", "toolset:gcc or toolset:clang" }
        buildoptions { 
            "-fsanitize=address",
            "-fsanitize=undefined",
        }
        linkoptions { 
            "-fsanitize=address",
            "-fsanitize=undefined",
        }
        -- Do Note: Production Builds have a problem with leak-Sanitizer
        -- Use the following before running your executable
        -- Linux:   export LSAN_OPTIONS=verbosity=1:log_threads=1
        -- Windows: $env:LSAN_OPTIONS="verbosity=1:log_threads=1"
        -- The actual solution would be to generate a run.sh/run.bat script every time I finish building
        -- Such that I'll be able to export the env-vars before executing my program
        -- But I don't need such fancy functionality, 
        -- especially when debug builds just work with leak-Sanitizer
    filter {}
    -- Specify Leak Sanitizer for Debug/Production (Unix/Posix only for now...)
    filter { "system:not windows", "configurations:Debug* or configurations:Production*", "toolset:gcc or toolset:clang" }
        buildoptions { 
            "-fsanitize=leak"
        }
        linkoptions { 
            "-fsanitize=leak"
        }
    filter {}



    -- Release Configuration Across Multiple Platforms
    filter { "configurations:Release*" }
        defines { "NDEBUG" }
        runtime  "Release"
        optimize "On"
        symbols  "Off"
    filter {}


    filter { "toolset:clang" }
        stl "libc++"
    
    filter { "toolset:gcc" }
        stl "gnu"
    filter {}

    filter { "toolset:gcc or toolset:clang" }
        buildoptions {
            "-Wshadow"
        }
    filter {}
        
