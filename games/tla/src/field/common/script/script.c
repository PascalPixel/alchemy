/// @file
/// Object scripts: move the instruction cursor and control byte54.

#include "script.h"

/// Move the script cursor and return 1 to continue with the next command.
s32 Script_JumpToNextOperandPointer(struct ScriptInterpreter *sc)
{
  s32 zero;

  // Jump to the script pointed to by the following word.
  sc->script = (const s32 *)sc->script[sc->cursor + 1];
  zero = 0;
  sc->cursor = zero;
  return 1;
}

/// Clear byte54 and advance to the next command.
s32 Script_ClearByte54(struct ScriptInterpreter *sc) {
  sc->byte54 = 0;
  sc->cursor++;
  return 1;
}

/// Set byte54 to 1 and advance to the next command.
s32 Script_SetByte54(struct ScriptInterpreter *sc) {
  sc->byte54 = 1;
  sc->cursor++;
  return 1;
}
