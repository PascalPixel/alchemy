#include "SCRIPT.H"
#include "FIELD_EVENT.H"

s32 Script_PlayAudioCue(struct ScriptInterpreter *interpreter)
{
    Audio_PlayCue(interpreter->script[interpreter->cursor + 1]);
    interpreter->cursor += 2;
    return 1;
}

/* Spanish and Italian keep this command's call at the English far-call
   address, which lies in their data, so they keep it as scaffold. */
#if !defined(TLA_EDITION_ES) && !defined(TLA_EDITION_IT)
s32 Func_080c88c8(struct ScriptInterpreter *interpreter, s32 value);

/* Script_PlayAudioCue's shape: passes the interpreter and its next operand
   to the far call at 0x080c88c8. */
s32 Script_CallWithOperand(struct ScriptInterpreter *interpreter)
{
    Func_080c88c8(interpreter, interpreter->script[interpreter->cursor + 1]);
    interpreter->cursor += 2;
    return 1;
}
#endif
