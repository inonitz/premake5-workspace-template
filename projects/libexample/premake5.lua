project "libexample"
    systemversion "latest"
    warnings      "extra"
    -- Project Structure
    files { 
        "include/**.h",
        "source/**.c",
        "include/**.hpp",
        "source/**.cpp"
    }
    filter {}
    -- Specify Include Headers
    includedirs { 
        "include" 
    }

    -- Build Directories &// Structure
    SetupBuildDirectoriesForLibrary()

    -- Build Options
    buildoptions {}
    filter {}

    -- Linking Options
    LinkToStandardLibraries()
    links {}
    
    -- Macros
    -- filter { "system:windows" }
    --     defines { "SYSTEM_WINDOWS" , "_CRT_SECURE_NO_WARNINGS" }
    filter {}
    filter { "configurations:*Lib" }
        defines { "LIBEXAMPLE_STATIC_DEFINE" }
    filter { "configurations:*Dll" }
        defines { "LIBEXAMPLE_EXPORTS" }
    filter {}


    -- Custom Pre &// Post build Actions