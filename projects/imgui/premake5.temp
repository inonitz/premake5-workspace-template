project "ImGui"
    GetLibraryType()
    language      "C++"
    cppdialect    "C++17"
    systemversion "latest"
    warnings      "extra"
    targetdir     (BIN_DIR)
    objdir        (OBJ_DIR)
    files {
        "include/**",
        "source/**",
    }
    includedirs {
        "include",
        DEPENDENCY_DIR .. "/GLFW/win64/include"
    }


    filter { "system:windows" }
        links { 
            "GLFW",
            "gdi32",
            "user32"
        }
    filter { "system:linux" }
        links { 
            "GLFW",
            "pthread",
            "dl", 
            "X11", 
            "Xxf86vm", 
            "Xrandr",
            "Xi",
            "Xinerama", 
            "Xcursor", 
        }
    
    
    filter { "system:windows", "kind:StaticLib" }
        libdirs { DEPENDENCY_DIR .. "/GLFW/%{cfg.architecture}/lib-static-ucrt" }
    filter { "system:windows", "kind:SharedLib" }
        libdirs { DEPENDENCY_DIR .. "/GLFW/%{cfg.architecture}/lib-vc2022" }
    -- on linux you'll probably build from source and install globally on your machine
    -- therefore, until I actively develop in a linux env, this'll be the expected folder structure
    filter { "system:linux", "kind:StaticLib" }
        libdirs { DEPENDENCY_DIR .. "/GLFW/%{cfg.architecture}/static" }
    filter { "system:linux", "kind:SharedLib" }
        libdirs { DEPENDENCY_DIR .. "/GLFW/%{cfg.architecture}/shared" }

