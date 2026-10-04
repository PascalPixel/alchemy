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

/* Spanish and Italian keep this command's call at the English far-call
   address, which lies in their data, so they keep it as scaffold. */
#if !defined(TLA_EDITION_ES) && !defined(TLA_EDITION_IT)
s32 Func_080c88c8(struct ScriptInterpreter *interpreter, s32 value);

/* Script_PlayAudioCue's shape: passes the interpreter and its next operand
   to the far call at 0x080c88c8. */
s32 Script_CallWithOperand(struct ScriptInterpreter *interpreter)
{
    s32 done;
    s32 cursor;

    Func_080c88c8(interpreter, interpreter->script[interpreter->cursor + 1]);
    cursor = (u16)interpreter->cursor;
    done = 1;
    /* FAKEMATCH: ⚓️ sets the result between the cursor load and store */
    asm volatile("" : "+l"(done));
    interpreter->cursor = cursor + 2;
    return done;
}
#endif
