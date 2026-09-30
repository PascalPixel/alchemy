#include "SCRIPT_INTERPRETER.H"

s32 GameFlag_Test(s32);
s32 GameFlag_SetBit(s32);

s32 Script_SetFlagAndTest(struct ScriptInterpreter *interpreter)
{
    s32 done;
    s32 cursor;
    s32 value;

    value = interpreter->script[interpreter->cursor + 1];
    interpreter->condition_result = GameFlag_Test(value);
    GameFlag_SetBit(value);
    cursor = (u16)interpreter->cursor;
    done = 1;
    asm volatile("" : "+l"(done)); /* FAKEMATCH: ⚓️ sets the result between the cursor load and store */
    interpreter->cursor = cursor + 2;
    return done;
}
