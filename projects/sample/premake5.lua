project "sample"
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
    IncludeProjectHeaders("libexample")


    -- Build Directories &// Structure
    SetupBuildDirectoriesForExecutable()

    -- Build Options
    buildoptions {
        -- "-march=native"
    }

    -- Linking Options 
    LinkToStandardLibraries()
    -- Other Project Library Links Defined here...
    -- e.g LinkProjectALibrary(...)
    LinkLibExampleLibrary()

    filter "system:windows"
        links { "shell32", "pthread" }
    filter "system:linux" 
        links { "dl", "pthread" }
    filter {}

    -- Macros
    defines {}


    -- Custom Pre &// Post build Actions
    PreBuildCopyBuildTargetCompileCommandsToFolder()
    PostBuildCommmandsForExecutable()