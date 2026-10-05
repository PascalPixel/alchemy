#include "SCRIPT.H"

s32 Script_SkipCommand(struct ScriptInterpreter *interpreter)
{
    interpreter->cursor += 2;
    return 1;
}
