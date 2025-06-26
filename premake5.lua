include "searchlocal.lua"
require(".vscode/export-compile-comms")
require(".vscode/premake-ninja")


WORKSPACE_NAME = "DefaultWorkspace"
PROJECT_LIST   = {
    "projects/benchmark194",
    "projects/sample"
}
START_PROJECT = "sample"


include "toolchain.lua"
include "workspace.lua"
include "dir.lua"
for _, path in ipairs(PROJECT_LIST) do
    include(path .. "/premake5.lua")
end
include "cmd.lua"