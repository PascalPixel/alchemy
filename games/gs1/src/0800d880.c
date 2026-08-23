#include "script_interpreter.h"

s32 GameFlag_IsSet(s32 arg0);
void GameFlag_Set(s32 arg0);
void GameFlag_Clear(s32 arg0);

s32 Func_0800d880(struct ScriptInterpreter *interpreter)
{
    s32 value;
    s32 result;

    value = interpreter->script[interpreter->cursor + 1];
    result = GameFlag_IsSet(value);
    interpreter->condition_result = result;
    if (((u32)result << 0x18) == 0x01000000) {
        GameFlag_Clear(value);
    } else {
        GameFlag_Set(value);
    }
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}
