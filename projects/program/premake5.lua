project "program-test"
    kind          "ConsoleApp"
    systemversion "latest"
    warnings      "extra"
    SpecifyGlobalProjectCXXVersion()
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
    filter "system:windows"
        links {
            "gdi32",
            "shell32"
        }
    filter "system:linux"
        links {
            "dl"
        }
    filter ""
    filter {}
    defines {}
    prebuildcommands {
        "ln -f %[../../compile_commands/%{cfg.shortname}.json] %[../../compile_commands/compile_commands.json]"
    }
