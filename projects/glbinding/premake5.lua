project "glbinding"
    language      "C++"
    cppdialect    "C++17"
    cdialect      "C11"
    systemversion "latest"
    warnings      "extra"
    files { 
        "include/**",
        "source/**" 
    }
    includedirs {
        "include",
        "source"        
    }
    SetupBuildDirectoriesForLibrary()
    filter {}
    

    defines {
        "_CRT_SECURE_NO_WARNINGS"
    }
    filter { "system:windows" }
        defines { "SYSTEM_WINDOWS" }
    filter {}
    filter { "configurations:*Lib" }
        defines { "GLBINDING_STATIC_DEFINE" }
    filter { "configurations:*Dll" }
        defines { "glbinding_EXPORTS" }
    filter {}