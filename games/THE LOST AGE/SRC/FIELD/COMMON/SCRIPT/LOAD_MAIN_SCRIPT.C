#include "TYPES.H"
#include "SCRIPT.H"

extern u16 Script_MainScript[];

s32 Script_LoadMainScript(struct ScriptInterpreter *interpreter)
{
    s32 result;

    interpreter->script = Script_MainScript;
    interpreter->cursor = (result = 0);
    return result;
}
