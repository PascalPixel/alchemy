#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "SCRIPT.H"
#include "FIELD_EVENT.H"

s32 Script_PlayAudioCue(struct ScriptInterpreter *interpreter)
{
    Audio_PlayCue(interpreter->script[interpreter->cursor + 1]);
    interpreter->cursor += 2;
    return 1;
}

#if !defined(TLA_EDITION_ES) && !defined(TLA_EDITION_IT)

s32 Script_CallWithOperand(struct ScriptInterpreter *interpreter)
{
    Func_080c88c8(interpreter, interpreter->script[interpreter->cursor + 1]);
    interpreter->cursor += 2;
    return 1;
}
#endif
