#include "script_interpreter.h"

#define Script_AdvancePastCursor Func_0800ca2c

s32 Script_AdvancePastCursor(struct ScriptInterpreter *interpreter)
{
    s32 zero;

    interpreter->script = interpreter->script + interpreter->cursor + 1;
    zero = 0;
    interpreter->cursor = zero;
    return 1;
}
