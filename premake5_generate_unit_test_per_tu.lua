-- This is taken from my WIP Packaging of GoogleTest/GoogleMock/GoogleBenchmark with premake5

-- Having a function to generate a project for each Test file is cumbersome,
-- Stupid, and frankly insane. How the hell am I supposed to figure out
-- that appearently there are inner-dependencies between these projects,
-- unless I manually tinker with each one until everything is compiles?
-- What the hell?
-- https://github.com/premake/premake-core/issues/998

defineTestProject = function(full_path_to_file, full_path_dir, name)
project(name)
    kind          "ConsoleApp"
    systemversion "latest"
    warnings      "extra"
    files { full_path_to_file }
    files { full_path_dir .. "/output_test_helper.cc" }
    IncludeProjectHeaders("benchmark194")
    IncludeProjectHeaders("googletest/googletest")
    IncludeProjectHeaders("googletest/googlemock")


    -- Build Directories &// Structure
    SetupBuildDirectoriesForExecutable()

    -- Build Options
    buildoptions {
        "-march=native"
    }

    -- Linking Options 
    LinkToStandardLibraries()
    -- Other Project Library Links Defined here...
    -- e.g LinkProjectALibrary(...)
    LinkGoogleBenchmarkLibrary()
    LinkGoogleTestLibrary()

    filter "system:windows"
        links { "shell32", "pthread" }
    filter "system:linux" 
        links { "dl", "pthread" }
    filter {}

    -- Macros
    defines {}

    -- Post build commands
    filter { "system:windows", "configurations:*Dll", "action:gmake" }
        if not os.isdir("../../" .. BUILD_BINARY_DIRECTORY) then
            postbuildcommands{ "mkdir -p %[../../%{BUILD_BINARY_DIRECTORY}]" }
            -- postbuildcommands{ "cp -rf %[../../%{BUILD_BINARY_DIRECTORY}_googletest/*   ] %[../../%{BUILD_BINARY_DIRECTORY}]" }
            -- postbuildcommands{ "cp -rf %[../../%{BUILD_BINARY_DIRECTORY}_benchmark194/* ] %[../../%{BUILD_BINARY_DIRECTORY}]" }
            -- postbuildcommands{ "cp -rf %[%{cfg.buildtarget.directory}] %[../../%{BUILD_BINARY_DIRECTORY}]" }
            postbuildcommands{ "cp -rf ../.." .. "%{BUILD_BINARY_DIRECTORY}_googletest/*   " .. "../.." .. "%{BUILD_BINARY_DIRECTORY}" }
            postbuildcommands{ "cp -rf ../.." .. "%{BUILD_BINARY_DIRECTORY}_benchmark194/* " .. "../.." .. "%{BUILD_BINARY_DIRECTORY}" }
            postbuildcommands{ "cp -rf %{cfg.buildtarget.directory}/* "                      .. "../.." .. "%{BUILD_BINARY_DIRECTORY}" }
        
        end
    filter {}
end


-- Thanks to: https://stackoverflow.com/questions/1426954/split-string-in-lua
function splitstr_by_char(inputstr, sep)
    if sep == nil then
        sep = "%s"
    end
    local t = {}
    for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
        table.insert(t, str)
    end
    return t
end

function getPath(str)
    return str:match("(.*[/\\])")
end



-- local realpath, err = os.realpath(fname)
-- if err then
--     printf("Error generating premake5 Test project - %s", err)
-- end
local test_files = os.matchfiles(_WORKING_DIR .. "/projects/sample/source/test/**.cc")  -- recursive match
for _, fname in ipairs(test_files) do
    local path_noext = string.match(fname, "(.+)%..+$")
    local all_words = splitstr_by_char(path_noext, "/")
    -- print(path_noext)
    -- print(all_words[#all_words])
    defineTestProject(fname, getPath(fname), all_words[#all_words])
end

-- project "sample"
--     kind          "ConsoleApp"
--     systemversion "latest"
--     warnings      "extra"
--     -- Project Structure
--     files {
--         "include/**.h",
--         "include/**.hpp",

--         "source/**.h",
--         "source/**.hpp",
--         "source/**.c",
--         "source/**.cc",
--         "source/**.cpp"
--     }
--     filter { "files:source/test/cxx11_test.cc" }
--         cppdialect "C++11"
--     filter {}
--     -- Specify Include Headers
--     -- Other Project Includes Defined here...
--     -- e.g IncludeProjectHeaders(...)
--     includedirs { "include", "source" }
--     IncludeProjectHeaders("libexample")
--     IncludeProjectHeaders("benchmark194")
--     IncludeProjectHeaders("googletest/googletest")
--     IncludeProjectHeaders("googletest/googlemock")


--     -- Build Directories &// Structure
--     SetupBuildDirectoriesForExecutable()

--     -- Build Options
--     buildoptions {
--         "-march=native"
--     }

--     -- Linking Options 
--     LinkToStandardLibraries()
--     -- Other Project Library Links Defined here...
--     -- e.g LinkProjectALibrary(...)
--     LinkLibExampleLibrary()
--     LinkGoogleBenchmarkLibrary()
--     LinkGoogleTestLibrary()

--     filter "system:windows"
--         links { "shell32", "pthread" }
--     filter "system:linux" 
--         links { "dl", "pthread" }
--     filter {}

--     -- Macros
--     defines {}


--     -- Custom Pre &// Post build Actions
--     prebuildcommands {
--         "{copyfile} %[../../.vscode/compile_commands/%{cfg.shortname}.json] %[../../.vscode/compile_commands/compile_commands.json]"
--     }
--     filter { "system:windows", "configurations:*Dll", "action:gmake" }
--         postbuildcommands {
--             "if not exist %[../../%{BUILD_BINARY_DIRECTORY}] mkdir  %[../../%{BUILD_BINARY_DIRECTORY}] \
--             {copydir} %[../../%{BUILD_BINARY_DIRECTORY}_libexample/*   ] %[../../%{BUILD_BINARY_DIRECTORY}] \
--             {copydir} %[../../%{BUILD_BINARY_DIRECTORY}_googletest/*   ] %[../../%{BUILD_BINARY_DIRECTORY}] \
--             {copydir} %[../../%{BUILD_BINARY_DIRECTORY}_benchmark194/* ] %[../../%{BUILD_BINARY_DIRECTORY}] \
--             {copydir} %[%{cfg.buildtarget.directory}] %[../../%{BUILD_BINARY_DIRECTORY}]"
--         }
--     filter {}
