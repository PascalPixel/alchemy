#include "script.h"
#include "object_runtime.h"

s32 Object_IsTargetUnset(struct ObjectRuntime *object);

s32 Script_Wait(struct ScriptInterpreter *interpreter) {
  s16 cursor = interpreter->cursor;

  interpreter->lookupResult = interpreter->script[cursor + 1] - 1;
  interpreter->cursor += 2;
  return 0;
}

/// Waits until the object reaches its target or 60 frames pass; on the
/// timeout the target is cleared. Returns 1 when the script moves on.
s32 Script_WaitForArrival(struct ScriptInterpreter *interpreter) {
  struct ObjectRuntime *object = (struct ObjectRuntime *)interpreter;
  u8 *delay = &interpreter->delay;

  if (*delay > 59) {
    /* FAKEMATCH: the 0 in a variable shares r2 between both byte stores, and
     * the delay's byte pointer keeps its store first; as the constant, 16
     * differ in tla-en, and through the field, 2. */
    u32 zero = 0;

    *delay = zero;
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    object->unk56 = zero;
    interpreter->cursor++;
    return 1;
  }
  if (Object_IsTargetUnset(object) != 0) {
    interpreter->cursor++;
    return 1;
  }
  return 0;
}
