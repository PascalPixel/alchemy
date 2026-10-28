/// @file
/// Object scripts: move the instruction cursor and control byte54.

#include "script.h"

/// Jumps to the script the next word points to and returns 1 to continue
/// with its first command.
s32 Script_JumpToNextOperandPointer(struct ScriptInterpreter *sc)
{
  s32 zero;

  // Jump to the script pointed to by the following word.
  sc->script = (const s32 *)sc->script[sc->cursor + 1];
  zero = 0;
  sc->cursor = zero;
  return 1;
}

/// Clears byte54 and advances to the next command.
s32 Script_ClearByte54(struct ScriptInterpreter *sc) {
  sc->byte54 = 0;
  sc->cursor++;
  return 1;
}

/// Sets byte54 to 1 and advances to the next command.
s32 Script_SetByte54(struct ScriptInterpreter *sc) {
  sc->byte54 = 1;
  sc->cursor++;
  return 1;
}
