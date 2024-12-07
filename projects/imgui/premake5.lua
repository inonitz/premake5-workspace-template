project "imgui"
    language      "C++"
    cppdialect    "C++17"
    cdialect      "C11"
    systemversion "latest"
    warnings      "extra"
    files { 
        "include/**.hpp",
        "source/**.cpp"
    }
    includedirs { 
        "include"
    }
    IncludeGLFWDirectory()
    

    SetupBuildDirectoriesForLibrary()
    filter {}
    filter "system:windows"
        links { 
            "gdi32",
            "user32"
        }
    filter { "system:linux" }
        links { 
            "pthread",
            "dl", 
            "X11", 
            "Xxf86vm", 
            "Xrandr",
            "Xi",
            "Xinerama", 
            "Xcursor", 
        }
    filter {}
    LinkGLFWLibrary()
    defines {}
