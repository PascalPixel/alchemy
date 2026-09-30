#include "SCRIPT_INTERPRETER.H"
extern u8 gObjectSlots[];

s32 GameFlag_TestFar(s32);
s32 GameFlag_SetBitFar(s32);
void GameFlag_ClearBitFar(s32);
void ObjectDispatch_ApplyArgumentToChildren(void *, s32);
void ObjectDispatch_Release(void);
s32 Audio_PlayCue(s32);

s32 Script_RepeatOrJump(struct ScriptInterpreter *interpreter)
{
    const s32 *arguments;
    s32 repeat_limit;
    s32 jump_key;
    u8 *repeat_count;
    s32 next_count;

    arguments = &interpreter->script[interpreter->cursor + 1];
    repeat_limit = *arguments++;
    jump_key = *arguments;
    if (repeat_limit == 0xFFFF) {
        interpreter->cursor = Script_FindLabel(interpreter, jump_key);
    } else {
        repeat_count = &interpreter->repeat_count;
        next_count = *repeat_count + 1;
        *repeat_count = next_count;
        if ((s32)(u8)next_count < (s32)(s16)repeat_limit) {
            interpreter->cursor = Script_FindLabel(interpreter, jump_key);
        } else {
            *repeat_count = 0;
            interpreter->cursor = interpreter->cursor + 3;
        }
    }
    return 1;
}
