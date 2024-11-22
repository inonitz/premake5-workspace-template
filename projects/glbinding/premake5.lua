project "glbinding"
    GetLibraryType()
    language      "C++"
    cppdialect    "C++17"
    cdialect      "C11"
    systemversion "latest"
    warnings      "extra"
    targetdir     (BIN_DIR)
    objdir        (OBJ_DIR)
    files {
        "include/**",
        "source/**",
    }
    includedirs {
        "include",
    }
    defines {
        "glbinding_EXPORTS"
    }
    filter { "system:windows" }
        defines { "SYSTEM_WINDOWS" }
    
    filter { "files:khrplatform.h" }
        cdialect "C11"
