#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "script.h"
#include "field_event.h"

s32 Script_PlayAudioCue(struct ScriptInterpreter *interpreter) {
  Audio_PlayCue(interpreter->script[interpreter->cursor + 1]);
  interpreter->cursor += 2;
  return 1;
}

#if !defined(TLA_EDITION_ES) && !defined(TLA_EDITION_IT)

s32 Script_CallWithOperand(struct ScriptInterpreter *interpreter) {
  Func_080c88c8(interpreter, interpreter->script[interpreter->cursor + 1]);
  interpreter->cursor += 2;
  return 1;
}
#endif
