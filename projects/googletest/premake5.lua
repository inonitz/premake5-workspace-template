project "googletest"
    systemversion "latest"
    warnings      "extra"
    -- Project Structure
    files { 
        "googlemock/src/gmock-all.cc",
        "googlemock/src/gmock_main.cc",
        "googletest/src/gtest-all.cc"
    }

    -- Specify Include Headers
    includedirs { 
        "googlemock/include",
        "googlemock",
        "googletest/include",
        "googletest"
    }

    -- Build Directories &// Structure
    SetupBuildDirectoriesForLibrary()

    -- Build Options
    buildoptions {
        "-pthread"
    }

    -- Linking Options
    LinkToStandardLibraries()
    links { "pthread" }
    
    -- Macros
    filter { "configurations:*Lib" }
        defines { "GTEST_STATIC_LIBRARY" }
    filter { "configurations:*Dll" }
        defines { "GTEST_CREATE_SHARED_LIBRARY" }
    filter {}


    -- Custom Pre &// Post build Actions