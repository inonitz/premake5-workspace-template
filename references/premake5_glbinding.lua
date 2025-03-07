project "glbinding"
    systemversion "latest"
    warnings      "extra"
    rtti          "On"
    SpecifyGlobalProjectCXXVersion()
    -- Project Structure
    files { 
        "include/**",
        "source/**" 
    }

    -- Specify Include Headers
    includedirs {
        "include",
        "source"        
    }

    -- Build Directories &// Structure
    SetupBuildDirectoriesForLibrary()

    -- Build Options

    -- Linking Options
    LinkToStandardLibraries()

    
    -- Macros
    filter { "system:windows" }
        defines { "SYSTEM_WINDOWS" , "_CRT_SECURE_NO_WARNINGS" }
    filter {}
    filter { "configurations:*Lib" }
        defines { "GLBINDING_STATIC_DEFINE" }
    filter { "configurations:*Dll" }
        defines { "glbinding_EXPORTS" }
    filter {}
    defines { 
        -- required for compilers using -std=c++20, -static-libstdc++ || clang_version < 19. 
        -- ==> https://github.com/msys2/MINGW-packages/issues/17730 [further read]
        "__GXX_TYPEINFO_EQUALITY_INLINE"
    }

    -- Custom Pre &// Post build Actions