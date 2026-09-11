#include "scene.h"
#include "abi/script/interpreter/cmd/load.h"
#include "script_interpreter.h"

extern const s32 gRom[];

s32 Script_LoadMainScript(struct ScriptInterpreter *interpreter)
{
    s32 result;

    interpreter->script = gRom;
    interpreter->cursor = (result = 0);
    return result;
}
