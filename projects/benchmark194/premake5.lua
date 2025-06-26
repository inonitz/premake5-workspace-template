project "benchmark194"
    systemversion "latest"
    warnings      "extra"
    -- Project Structure
    files { 
        "include/**.h",
        "src/**.h",
        "src/**.cc"
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

    -- Linking Options
    LinkToStandardLibraries()
    filter "system:windows"
        links { "shlwapi" }
    filter {}
    
    links {}
    
    
    -- Macros
    -- filter { "system:windows" }
    --     defines { "SYSTEM_WINDOWS" , "_CRT_SECURE_NO_WARNINGS" }
    filter {}
    filter { "configurations:*Lib" }
        defines { "BENCHMARK_STATIC_DEFINE" }
    filter { "configurations:*Dll" }
        defines { "benchmark_EXPORTS" }
    filter {}


    -- Custom Pre &// Post build Actions