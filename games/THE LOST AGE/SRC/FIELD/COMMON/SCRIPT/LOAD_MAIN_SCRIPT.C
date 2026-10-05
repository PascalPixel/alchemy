#include "GLOBAL.H"
#include "TYPES.H"
#include "SCRIPT.H"


s32 Script_LoadMainScript(struct ScriptInterpreter *interpreter)
{
    s32 result;

    interpreter->script = Script_MainScript;
    interpreter->cursor = (result = 0);
    return result;
}
