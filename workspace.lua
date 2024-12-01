workspace (WORKSPACE_NAME)
    startproject(START_PROJECT)
    configurations {
        "Debug",
        "Release"
    }
    platforms {
        "Static",
        "Shared"
    }


    -- default system parameters, required for proper folder structure.
    system          "windows"  -- can be overriden using --os='x'   flag
    architecture    "x86_64"   -- can be overriden using --arch='x' flag
    -- print(_OPTIONS["arch"])
    filter "options:arch=universal_ios_macos_binary" 
        architecture "universal"
    filter "options:arch=x86 or options:arch=i386"
        architecture "x86"
    filter "options:arch=x86_64 or options:arch=amd64"
        architecture "x86_64"
    filter "options:arch=ARM"
        architecture "ARM"
    filter "options:arch=ARM64"
        architecture "ARM64"
    filter "options:arch=armv5"  
        architecture "armv5" 
    filter "options:arch=armv7"  
        architecture "armv7" 
    filter "options:arch=aarch64"
        architecture "aarch64"
    filter "options:arch=mips"   
        architecture "mips"  
    filter "options:arch=mips64" 
        architecture "mips64"


    filter "configurations:*Debug"
        defines { "DEBUG" }
        runtime  "Debug"
        symbols  "on"
        optimize "off"
    filter "configurations:*Release"
        defines { "NDEBUG" }
        runtime  "Release"
        symbols  "off"
        optimize "on"


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
