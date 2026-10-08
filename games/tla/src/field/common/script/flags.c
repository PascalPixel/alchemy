#include "script.h"

s32 GameFlag_Test(s32);
s32 GameFlag_SetBit(s32);
void GameFlag_ClearBit(s32);

s32 Script_SetFlagAndTest(struct ScriptInterpreter *interpreter)
{
    s32 value;

    value = interpreter->script[interpreter->cursor + 1];
    interpreter->condition_result = GameFlag_Test(value);
    GameFlag_SetBit(value);
    interpreter->cursor += 2;
    return 1;
}

s32 Script_ClearFlagAndTest(struct ScriptInterpreter *interpreter)
{
    s32 value;

    value = interpreter->script[interpreter->cursor + 1];
    interpreter->condition_result = GameFlag_Test(value);
    GameFlag_ClearBit(value);
    interpreter->cursor += 2;
    return 1;
}
