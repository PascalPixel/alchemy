#include "script.h"
#include "actor.h"

s32 Actor_IsTargetUnset(struct Actor *object);

/// The actor as SC_SYNC reads it: the frames a solid object has blocked it
/// are a byte of the actor itself, which struct Actor leaves unnamed.
struct WaitingActor {
  u8 unk00[0x38];
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  u8 unk44[0x12];
  u8 unk56;
  u8 unk57[9];
  u8 delay;
};

s32 Script_Wait(struct ScriptInterpreter *interpreter) {
  s16 cursor = interpreter->cursor;

  interpreter->lookupResult = interpreter->script[cursor + 1] - 1;
  interpreter->cursor += 2;
  return 0;
}

/// Waits until the object reaches its target or 60 frames pass; on the
/// timeout the target is cleared. Returns 1 when the script moves on.
s32 Script_WaitForArrival(struct ScriptInterpreter *interpreter) {
  struct WaitingActor *object = (struct WaitingActor *)interpreter;

  if (object->delay > 59) {
    object->delay = 0;
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    object->unk56 = 0;
    interpreter->cursor++;
    return 1;
  }
  if (Actor_IsTargetUnset((struct Actor *)interpreter) != 0) {
    interpreter->cursor++;
    return 1;
  }
  return 0;
}
