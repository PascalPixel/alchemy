#include "SCRIPT.H"

s32 GameFlag_Test(s32);

s32 Script_TestFlag(struct ScriptInterpreter *interpreter)
{
    s32 done;

    interpreter->condition_result =
        GameFlag_Test(interpreter->script[interpreter->cursor + 1]);
    done = 1;
    asm volatile("" : "+l"(done)); /* FAKEMATCH: ⚓️ sets the result before the cursor update */
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return done;
}
