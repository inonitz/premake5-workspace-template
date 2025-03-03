project "program"
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
    includedirs { "include", "source" }
    IncludeProjectHeaders("util")
    IncludeProjectHeaders("glbinding")
    IncludeProjectHeaders("glbinding-aux")
    IncludeProjectHeaders("imgui")
    IncludeProjectHeaders("awc2")

    -- Build Directories &// Structure
    SetupBuildDirectoriesForExecutable()

    -- Build Options
    

    -- Linking Options 
    LinkToStandardLibraries()
    LinkUtilLibrary()
    LinkGLBindingLibraries()
    LinkImGuiLibrary()
    LinkAWC2Library()
    LinkGLFWLibrary()
    filter "system:windows"
        links { "gdi32", "shell32" }
    filter "system:linux" 
        links { "dl", "pthread" }
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
            {copyfile} %[%{DEPENDENCY_DIR}/GLFW/windows/x86_64/lib-vc2022/glfw3.dll] %[../../%{BUILD_BINARY_DIRECTORY}] \
            {copyfile} %[%{DEPENDENCY_DIR}/GLFW/windows/x86_64/lib-vc2022/glfw3dll.lib] %[../../%{BUILD_BINARY_DIRECTORY}] \
            {copydir} %[../../%{BUILD_BINARY_DIRECTORY}_util/*         ] %[../../%{BUILD_BINARY_DIRECTORY}] \
            {copydir} %[../../%{BUILD_BINARY_DIRECTORY}_glbinding/*    ] %[../../%{BUILD_BINARY_DIRECTORY}] \
            {copydir} %[../../%{BUILD_BINARY_DIRECTORY}_glbinding-aux/*] %[../../%{BUILD_BINARY_DIRECTORY}] \
            {copydir} %[../../%{BUILD_BINARY_DIRECTORY}_imgui/*        ] %[../../%{BUILD_BINARY_DIRECTORY}] \
            {copydir} %[../../%{BUILD_BINARY_DIRECTORY}_awc2/*         ] %[../../%{BUILD_BINARY_DIRECTORY}] \
            {copydir} %[%{cfg.buildtarget.directory}] %[../../%{BUILD_BINARY_DIRECTORY}]"
        }
    -- Just thought about it and linux doesn't need the .so libs in the same folder, but instead needs to be installed on /usr/bin or /usr/lib
    -- 
    -- filter { "action:gmake2", "system:linux", "configurations:*Dll" }
    --     postbuildcommands {
    --         "mkdir -p %[../../%{BUILD_BINARY_DIRECTORY}] \
    --         {copydir} %[../../%{BUILD_BINARY_DIRECTORY}_util/*         ] %[../../%{BUILD_BINARY_DIRECTORY}] \
    --         {copydir} %[../../%{BUILD_BINARY_DIRECTORY}_glbinding/*    ] %[../../%{BUILD_BINARY_DIRECTORY}] \
    --         {copydir} %[../../%{BUILD_BINARY_DIRECTORY}_glbinding-aux/*] %[../../%{BUILD_BINARY_DIRECTORY}] \
    --         {copydir} %[../../%{BUILD_BINARY_DIRECTORY}_imgui/*        ] %[../../%{BUILD_BINARY_DIRECTORY}] \
    --         {copydir} %[../../%{BUILD_BINARY_DIRECTORY}_awc2/*         ] %[../../%{BUILD_BINARY_DIRECTORY}] \
    --         {copydir} %[%{cfg.buildtarget.directory}/*] %[../../%{BUILD_BINARY_DIRECTORY}]"
    --     }
    filter {}
