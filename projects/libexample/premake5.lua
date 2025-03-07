project "libexample"
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

    
    -- Macros
    filter { "configurations:*Lib" }
        defines { "LIBEXAMPLE_STATIC_DEFINE" }
    filter { "configurations:*Dll" }
        defines { "LIBEXAMPLE_EXPORTS" }
    filter {}

    -- Custom Pre &// Post build Actions