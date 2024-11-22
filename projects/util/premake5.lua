project "libutil"
    GetLibraryType()
    language      "C++"
    cppdialect    "C++17"
    buildoptions {
        "-msse3"
    }
    systemversion "latest"
    warnings      "extra"
    targetdir     (BIN_DIR)
    objdir        (OBJ_DIR)
    files {
        "include/**.hpp",
        "source/**.cpp"
    }
    includedirs {
        "include"
    }
