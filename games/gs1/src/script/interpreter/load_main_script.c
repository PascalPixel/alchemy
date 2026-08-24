#include "script_interpreter.h"

extern const s32 Data_08013240[];

s32 Script_LoadMainScript(struct ScriptInterpreter *interpreter)
{
    s32 result;

    interpreter->script = Data_08013240;
    interpreter->cursor = (result = 0);
    return result;
}
