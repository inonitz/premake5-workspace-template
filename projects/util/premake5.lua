project "util"
    language      "C++"
    cppdialect    "C++17"
    cdialect      "C11"
    systemversion "latest"
    warnings      "extra"
    files { 
        "include/**.h",
        "source/**.c",
        "include/**.hpp",
        "source/**.cpp"
    }
    includedirs { "include" }
    SetupBuildDirectoriesForLibrary()
    filter ""
    filter "toolset:clang"
        buildoptions {
            "-msse3"
        }
    links {}
    filter { "configurations:*Lib" }
        defines { "UTIL_STATIC_DEFINE" }
    filter { "configurations:*Dll" }
        defines { "UTIL_EXPORTS" }
    filter {}