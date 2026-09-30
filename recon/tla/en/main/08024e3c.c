#include "SCRIPT_INTERPRETER.H"
extern u8 gObjectSlots[];

s32 GameFlag_TestFar(s32);
s32 GameFlag_SetBitFar(s32);
void GameFlag_ClearBitFar(s32);
void ObjectDispatch_ApplyArgumentToChildren(void *, s32);
void ObjectDispatch_Release(void);
s32 Audio_PlayCue(s32);

s32 Script_SetFlagAndTest(struct ScriptInterpreter *interpreter)
{
    s32 value;

    value = interpreter->script[interpreter->cursor + 1];
    interpreter->condition_result = GameFlag_TestFar(value);
    GameFlag_SetBitFar(value);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}
