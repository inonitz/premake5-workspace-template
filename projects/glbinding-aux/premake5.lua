project "glbinding-aux"
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
        ROOT_PATH "projects/glbinding/include",
        "include",
    }
    defines {
        "glbinding_aux_EXPORTS"
    }    
    filter { "system:windows" }
        defines { "SYSTEM_WINDOWS" }

    
    filter { "files:khrplatform.h" }
        cdialect "C11"
