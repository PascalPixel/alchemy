#include "SCRIPT.H"

s32 Audio_PlayCue(s32);

s32 Script_PlayAudioCue(struct ScriptInterpreter *interpreter)
{
    s32 done;
    s32 cursor;

    Audio_PlayCue(interpreter->script[interpreter->cursor + 1]);
    cursor = (u16)interpreter->cursor;
    done = 1;
    asm volatile("" : "+l"(done)); /* FAKEMATCH: ⚓️ sets the result between the cursor load and store */
    interpreter->cursor = cursor + 2;
    return done;
}
