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
        "-march=native"
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
    prebuildcommands {
        "{copyfile} %[../../.vscode/compile_commands/%{cfg.shortname}.json] %[../../.vscode/compile_commands/compile_commands.json]"
    }
    filter { "system:windows", "configurations:*Dll", "action:gmake" }
        postbuildcommands {
            "if not exist %[../../%{BUILD_BINARY_DIRECTORY}] mkdir  %[../../%{BUILD_BINARY_DIRECTORY}] \
            {copydir} %[../../%{BUILD_BINARY_DIRECTORY}_libexample/* ] %[../../%{BUILD_BINARY_DIRECTORY}] \
            {copydir} %[%{cfg.buildtarget.directory}] %[../../%{BUILD_BINARY_DIRECTORY}]"
        }
    filter {}
