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
#include "edition.h"

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
// The Spanish and Italian far-call tables use a different name for the
// operand call entry.
s32 Func_080c88b8(struct ScriptInterpreter *interpreter, s32 value);
#define Func_080c88c8 Func_080c88b8
#endif

s32 Script_PlaySound(struct ScriptInterpreter *interpreter) {
  Audio_PlayCue(interpreter->script[interpreter->cursor + 1]);
  interpreter->cursor += 2;
  return 1;
}

s32 Script_CallWithOperand(struct ScriptInterpreter *interpreter) {
  Func_080c88c8(interpreter, interpreter->script[interpreter->cursor + 1]);
  interpreter->cursor += 2;
  return 1;
}
