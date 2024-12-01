project "example-mdk"
    kind "WindowedApp"
    systemversion "latest"
    language      "C++"
    cppdialect    "C++17"
    cdialect      "C11"
    warnings      "extra"
    buildoptions {
        "-municode",
        "-pedantic",
        "-Werror",
        "-Wall",
        "-march=native",
        "-mtune=native"
    }
    targetdir (BIN_DIR)
    objdir    (OBJ_DIR)
    files {
        "source/**"
    }


    includedirs {
        ROOT_PATH "projects/util/include"
    }
    links {
        "util",
        "lib"
    }
    filter "platforms:Static" -- for a static build we only need to hook the awc2 lib
        staticruntime "on"
    filter "platforms:Shared" -- for a dynamic build we need to hook all the required shared libraries.
        staticruntime "off"
    