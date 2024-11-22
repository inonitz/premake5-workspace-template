project "awc2"
    GetLibraryType()
    language      "C++"
    cppdialect    "C++17"
    staticruntime "on"
    systemversion "latest"
    warnings      "extra"
    targetdir     (BIN_DIR)
    objdir        (OBJ_DIR)
    files {
        "include/**.hpp",
        "source/**.cpp",
    }
    includedirs {
        ROOT_PATH "projects/glbinding/include",
        ROOT_PATH "projects/glbinding-aux/include",
        ROOT_PATH "projects/ImGui/include",
        ROOT_PATH "projects/util/include",
        DEPENDENCY_DIR .. "/GLFW/win64/include",
        "include"
    }
    links {
        "glbinding",
        "glbinding-aux",
        "ImGui",
        "libutil"
    }
    filter "kind:SharedLib"
        links {
            "GLFW"
        }
        libdirs { DEPENDENCY_DIR .. "/GLFW/glfw3.4/lib-vc2022" }