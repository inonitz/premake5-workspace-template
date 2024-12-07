project "awc2"
    language      "C++"
    cppdialect    "C++17"
    cdialect      "C11"
    systemversion "latest"
    warnings      "extra"
    files {
        "include/**.hpp",
        "source/**.cpp"
    }
    includedirs { "include" }
    IncludeProjectHeaders("util")
    IncludeProjectHeaders("glbinding")
    IncludeProjectHeaders("glbinding-aux")
    IncludeProjectHeaders("imgui")
    IncludeGLFWDirectory()
    
    SetupBuildDirectoriesForLibrary()

    LinkUtilLibrary()
    LinkGLBindingLibraries()
    LinkProjectLibrary("imgui")
    LinkGLFWLibrary()
    filter {}
    defines {}
