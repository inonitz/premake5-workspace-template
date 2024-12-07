project "glbinding-aux"
    language      "C++"
    cppdialect    "C++17"
    cdialect      "C11"
    systemversion "latest"
    warnings      "extra"
    files { 
        "include/**",
        "source/**.cpp" 
    }
    includedirs { "include" }
    IncludeProjectHeaders("glbinding")
    SetupBuildDirectoriesForLibrary()
    filter {}
    

    LinkProjectLibrary("glbinding")
    defines {
        "_CRT_SECURE_NO_WARNINGS"
    }    
    filter { "system:windows" }
        defines { "SYSTEM_WINDOWS" }
    filter {}
    filter { "configurations:*Lib" }
        defines { "GLBINDING_STATIC_DEFINE", "GLBINDING_AUX_STATIC_DEFINE" }
    filter { "configurations:*Dll" }
        defines { "glbinding_aux_EXPORTS" }
    filter {}