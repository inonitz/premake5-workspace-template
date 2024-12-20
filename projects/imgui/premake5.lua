project "imgui"
    SpecifyGlobalProjectCXXVersion()
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
    filter "system:windows"
        links { 
            "gdi32",
            "user32"
        }
    filter "system:linux"
        links { 
            "pthread",
            "dl", 
            "X11", 
            "Xrandr"
        }
    filter ""
    LinkGLFWLibrary()
    defines {}
