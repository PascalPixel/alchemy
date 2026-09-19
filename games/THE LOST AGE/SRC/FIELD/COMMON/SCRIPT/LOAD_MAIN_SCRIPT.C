#include "TYPES.H"
#include "SCRIPT_INTERPRETER.H"

extern u16 Data_0802ec48[];

s32 Script_LoadMainScript(struct ScriptInterpreter *interpreter)
{
    s32 result;

    interpreter->script = Data_0802ec48;
    interpreter->cursor = (result = 0);
    return result;
}
