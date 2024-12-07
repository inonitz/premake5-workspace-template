project "program-test"
    kind          "ConsoleApp"
    language      "C++"
    cppdialect    "C++17"
    cdialect      "C11"
    systemversion "latest"
    warnings      "extra"
    files {
        "include/**.hpp",
        "source/**.hpp",
        "source/**.cpp"
    }
    includedirs { "include", "source" }
    IncludeProjectHeaders("util")
    IncludeProjectHeaders("glbinding")
    IncludeProjectHeaders("glbinding-aux")
    IncludeProjectHeaders("imgui")
    IncludeProjectHeaders("awc2")
    IncludeGLFWDirectory()

    SetupBuildDirectoriesForExecutable()

    LinkUtilLibrary()
    LinkGLBindingLibraries()
    LinkProjectLibrary("imgui")
    LinkProjectLibrary("awc2")
    LinkGLFWLibrary()
    defines {}
