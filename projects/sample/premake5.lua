project "sampleprogram"
    kind          "ConsoleApp"
    systemversion "latest"
    warnings      "extra"
    SpecifyGlobalProjectCXXVersion()
    -- Project Structure
    files {
        "include/**.hpp",
        "source/**.hpp",
        "source/**.cpp"
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
        links { "shell32" }
    filter "system:linux" 
        links { "dl" }
    filter {}

    -- Macros
    defines {}


    -- Custom Pre &// Post build Actions
    prebuildcommands {
        "{copyfile} %[../../.vscode/compile_commands/%{cfg.shortname}.json] %[../../.vscode/compile_commands/compile_commands.json]"
    }
    filter { "action:gmake2", "system:windows", "configurations:*Dll" }
        postbuildcommands {
            "if not exist %[../../%{BUILD_BINARY_DIRECTORY}] mkdir  %[../../%{BUILD_BINARY_DIRECTORY}] \
            {copydir} %[../../%{BUILD_BINARY_DIRECTORY}_libexample/*   ] %[../../%{BUILD_BINARY_DIRECTORY}] \
            {copydir} %[%{cfg.buildtarget.directory}] %[../../%{BUILD_BINARY_DIRECTORY}]"
        }
    filter {}
