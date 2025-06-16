include "searchlocal.lua"
require(".vscode/export-compile-comms")


WORKSPACE_NAME = "DefaultWorkspace"
PROJECT_LIST   = {
    "projects/util2",
    "projects/prog"
}
START_PROJECT = "prog"


include "toolchain.lua"
include "workspace.lua"
include "dir.lua"
for _, path in ipairs(PROJECT_LIST) do
    include(path .. "/premake5.lua")
end
include "cmd.lua"