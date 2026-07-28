#include "script_interpreter.h"

s8 Func_080770c0(s32);
s32 Func_080770c8(s32);

s32 Func_0800d820(struct ScriptInterpreter *interpreter)
{
    s32 value;

    value = interpreter->script[interpreter->cursor + 1];
    interpreter->condition_result = Func_080770c0(value);
    Func_080770c8(value);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}
