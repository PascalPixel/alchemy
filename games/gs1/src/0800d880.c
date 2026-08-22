#include "script_interpreter.h"

s32 Func_080770c0(s32 arg0);
void Func_080770c8(s32 arg0);
void Func_080770d0(s32 arg0);

s32 Func_0800d880(struct ScriptInterpreter *interpreter)
{
    s32 value;
    s32 result;

    value = interpreter->script[interpreter->cursor + 1];
    result = Func_080770c0(value);
    interpreter->condition_result = result;
    if (((u32)result << 0x18) == 0x01000000) {
        Func_080770d0(value);
    } else {
        Func_080770c8(value);
    }
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}
