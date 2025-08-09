project "program"
    kind          "ConsoleApp"
    systemversion "latest"
    warnings      "extra"
    -- Project Structure
    files {
        "include/**.hpp",
        "include/**.h",
        "source/**.hpp",

        "source/**.cpp",

        "source/**.h",
        "source/**.c"
    }
    -- Specify Include Headers
    -- Other Project Includes Defined here...
    -- e.g IncludeProjectHeaders(...)
    includedirs { "include", "source" }
    IncludeDependencyHeaders(os.getenv("HIP_PATH"))
    IncludeProjectHeaders("util2")

    -- Build Directories &// Structure
    SetupBuildDirectoriesForExecutable()

    -- Build Options
    buildoptions {
        "-march=native"
    }

    -- Linking Options 
    LinkToStandardLibraries()
    -- Other Project Library Links Defined here...
    -- e.g LinkProjectALibrary(...)
    LinkUtil2Library()
    filter "system:windows"
        links { "gdi32", "shell32" }
    filter "system:linux" 
        links { "dl", "pthread" }
    filter {}

    -- Macros
    defines {}
    PreBuildCopyBuildTargetCompileCommandsToFolder()
    PostBuildCommmandsForExecutable()
    