project "util"
    SpecifyGlobalProjectCXXVersion()
    systemversion "latest"
    warnings      "extra"
    files { 
        "include/**.h",
        "source/**.c",
        "include/**.hpp",
        "source/**.cpp"
    }


    includedirs { 
        "include" 
    }
    SetupBuildDirectoriesForLibrary()
    filter "toolset:clang or toolset:gcc"
        buildoptions {
            "-msse3"
        }
    filter ""
    filter "files:stb_image.h"
        warnings "Off"
    filter ""
    links {}
    filter { "configurations:*Lib" }
        defines { "UTIL_STATIC_DEFINE" }
    filter { "configurations:*Dll" }
        defines { "UTIL_EXPORTS" }
    filter {}
