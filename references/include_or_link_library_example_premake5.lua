IncludeProjectHeaders = function(ProjectName)
    includedirs { _MAIN_SCRIPT_DIR .. "/projects/" .. ProjectName .. "/include" }
end


LinkProjectLibrary = function(ProjectName)
    links { ProjectName }
end


LinkUtil2Library = function()
    LinkProjectLibrary("util2")
    filter { "configurations:*Lib" }
        defines { "UTIL2_STATIC_DEFINE" }
    filter {}
end

LinkGLFWLibrary = function()
    LinkProjectLibrary("glfw34")
    filter { "system:windows", "configurations:*Lib" }
        links { 
            "user32",
            "imm32",
            "gdi32",
            "shell32"
        }
    filter { "system:linux", "configurations:*Lib" }
        links { 
            "pthread",
            "dl", 
            "X11", 
        }
    filter {}
end

LinkGLBindingLibraries = function()
    LinkProjectLibrary("glbinding")
    filter { "configurations:*Lib" }
        defines { "GLBINDING_STATIC_DEFINE" }
    filter {}
    LinkProjectLibrary("glbinding-aux")
    filter { "configurations:*Lib" }
        defines { "GLBINDING_AUX_STATIC_DEFINE" }
    filter {}
end

LinkImGuiLibrary = function()
    LinkProjectLibrary("imgui")
    filter { "configurations:*Lib" }
        defines { "IMGUI_STATIC_DEFINE" }

    filter { "configurations:*Lib", "system:windows" }
        links { "imm32" }

    filter {}
end

LinkAWC2Library = function()
    LinkProjectLibrary("awc2")
    filter { "configurations:*Lib" }
        defines { "AWC2_STATIC_DEFINE" }
    filter {}
end


LinkLibExampleLibrary = function()
    LinkProjectLibrary("libexample")
    filter { "configurations:*Lib" }
        defines { "LIBEXAMPLE_STATIC_DEFINE" }
    filter {}
end