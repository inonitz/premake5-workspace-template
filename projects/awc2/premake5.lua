project "awc2"
    systemversion "latest"
    warnings      "extra"
    rtti          "On"
    SpecifyGlobalProjectCXXVersion()
    -- Project Structure
    files {
        "include/**.h",
        "include/**.c",
        "include/**.hpp",
        "source/**.cpp"
    }

    -- Specify Include Headers
    includedirs { "include" }
    IncludeProjectHeaders("util")
    IncludeProjectHeaders("glbinding")
    IncludeProjectHeaders("glbinding-aux")
    IncludeProjectHeaders("imgui")
    IncludeGLFWDirectory()

    -- Build Directories &// Structure
    SetupBuildDirectoriesForLibrary()

    -- Build Options
    filter { "files:**.h", "files:**.c" }
        cdialect "C11"
    filter { "files:**.hpp", "files:**.cpp" }
        cppdialect "C++17"
    filter {}

    -- Linking Options
    LinkToStandardLibraries()
    LinkUtilLibrary()
    LinkGLBindingLibraries()
    LinkImGuiLibrary()
    LinkGLFWLibrary()
    
    -- Macros
    filter { "configurations:*Lib" }
        defines { "AWC2_STATIC_DEFINE" }
    filter { "configurations:*Dll" }
        defines { "AWC2_EXPORTS" }
    filter {}

    -- Custom Pre &// Post build Actions