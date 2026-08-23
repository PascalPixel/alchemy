#include "script_interpreter.h"

s8 GameFlag_IsSet(s32);
s32 GameFlag_Set(s32);

s32 Func_0800d820(struct ScriptInterpreter *interpreter)
{
    s32 value;

    value = interpreter->script[interpreter->cursor + 1];
    interpreter->condition_result = GameFlag_IsSet(value);
    GameFlag_Set(value);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}
