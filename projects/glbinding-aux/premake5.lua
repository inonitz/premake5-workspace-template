project "glbinding-aux"
    SpecifyGlobalProjectCXXVersion()
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
    filter { "system:linux" }
        links { "pthread" }
    filter {} 
    filter { "system:windows" }
        defines { "SYSTEM_WINDOWS" , "_CRT_SECURE_NO_WARNINGS" }
    filter {}
    filter { "configurations:*Lib" }
        defines { "GLBINDING_STATIC_DEFINE", "GLBINDING_AUX_STATIC_DEFINE" }
    filter { "configurations:*Dll" }
        defines { "glbinding_aux_EXPORTS" }
    filter {}