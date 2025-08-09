include ".vscode/premake5-lua/searchlocal.lua"
require(".vscode/export-compile-comms")
require(".vscode/premake-ninja")


WORKSPACE_NAME = "DefaultWorkspace"
PROJECT_LIST   = {
    "projects/util2",
    "projects/program"
}
START_PROJECT = "program"


include ".vscode/premake5-lua/toolchain.lua"
include "workspace.lua"
include ".vscode/premake5-lua/dir.lua"
for _, path in ipairs(PROJECT_LIST) do
    include(path .. "/premake5.lua")
end
include ".vscode/premake5-lua/cmd.lua"