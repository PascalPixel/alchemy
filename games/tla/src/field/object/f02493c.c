#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"

#define TARGET_UNSET ((s32)0x80000000)

struct MotionObject {
  void *script;
  u16 scriptCursor;
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  s32 terrainHeight;
  u8 unk18[0x0c];
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 speedLimit;
  s32 acceleration;
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  s32 bounce;
  s32 gravity;
  u8 unk4c[9];
  u8 motionFlags;
  u8 arrivalAxis;
  u8 unk57;
  u8 snapToTarget;
  u8 collisionFlags;
  u8 steeringFlags;
  u8 paused;
  u8 unk5c[5];
  u8 frozen;
  u8 unk62[0x1e];
};

void ObjectList_CollectActive(void *objects, struct MotionObject **out);
s32 ArcTan2(s32 y, s32 x);

void Func_0802493c(void) {
  struct MotionObject *list[15];
  struct MotionObject **next;
  struct MotionObject *object;
  s32 y;
  s32 arrivedAtTarget;
  s32 x;
  s32 z;
  s32 dx;
  s32 dz;
  s32 distance;
  s32 ratio;
  s32 facingStep;

  object = Ram_HeapSlots->scriptObjects;
  ObjectList_CollectActive(object, list);
  object = list[0];
  next = &list[1];
  while (object != NULL) {
    x = object->x;
    y = object->y;
    z = object->z;
    if (object->frozen == 0) {
      arrivedAtTarget = 0;
      if (object->targetX != TARGET_UNSET) {
        dx = (object->targetX - x) / 65536;
        dz = (object->targetZ - z) / 65536;
        distance = Iwram_Sqrt(dx * dx + dz * dz) << 16;
        if (distance <= 0xffffff) {
          dx = object->targetX - x;
          dz = object->targetZ - z;
          distance = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz))
                     << 8;
        }
        if (distance == 0) {
          x = object->targetX;
          z = object->targetZ;
        } else {
          ratio = Iwram_RatioMulQ14(distance, object->acceleration);
          object->velocityX += Iwram_MulQ16(dx, ratio);
          object->velocityZ += Iwram_MulQ16(dz, ratio);
          dx = object->velocityX;
          dz = object->velocityZ;
          distance = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz))
                     << 8;
          if (distance > object->speedLimit) {
            ratio = Iwram_RatioMulQ14(distance, object->speedLimit);
            object->velocityX = Iwram_MulQ16(dx, ratio);
            object->velocityZ = Iwram_MulQ16(dz, ratio);
          }
        }
      } else {
        dx = object->velocityX;
        dz = object->velocityZ;
        if ((dx | dz) != 0) {
          distance = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz))
                     << 8;
          if (distance != 0) {
            if (distance - object->acceleration < 0) {
              object->velocityX = 0;
              object->velocityZ = 0;
            } else {
              ratio =
                  Iwram_RatioMulQ14(distance, distance - object->acceleration);
              object->velocityX = Iwram_MulQ16(dx, ratio);
              object->velocityZ = Iwram_MulQ16(dz, ratio);
            }
          } else {
            object->velocityX = 0;
            object->velocityZ = 0;
          }
        }
      }
      if (object->motionFlags & 2) {
        if (y > object->terrainHeight) {
          object->velocityY -= object->gravity;
        } else if (object->velocityY < 0) {
          y = object->terrainHeight;
          object->velocityY = -Iwram_MulQ16(object->velocityY, object->bounce);
          if ((object->velocityY < 0 ? -object->velocityY
                                     : object->velocityY) <= object->gravity)
            object->velocityY = 0;
        }
      }
    }
    x += object->velocityX;
    y += object->velocityY;
    z += object->velocityZ;
    if (object->arrivalAxis != 0)
      switch (object->arrivalAxis) {
      case 16:
        if (x == object->targetX ||
            ((object->x - object->targetX) ^ (x - object->targetX)) < 0)
          arrivedAtTarget = 1;
        break;
      case 17:
        if (y == object->targetY ||
            ((object->y - object->targetY) ^ (y - object->targetY)) < 0)
          arrivedAtTarget = 1;
        break;
      case 18:
        if (z == object->targetZ ||
            ((object->z - object->targetZ) ^ (z - object->targetZ)) < 0)
          arrivedAtTarget = 1;
        break;
      }
    if (arrivedAtTarget) {
      if (object->snapToTarget) {
        object->velocityX = 0;
        object->velocityZ = 0;
        x = object->targetX;
        z = object->targetZ;
        if (object->motionFlags == 0) {
          y = object->targetY;
          object->velocityY = 0;
        }
      }
      object->targetX = TARGET_UNSET;
      object->targetY = TARGET_UNSET;
      object->targetZ = TARGET_UNSET;
      object->arrivalAxis = 0;
    }
    object->x = x;
    object->y = y;
    object->z = z;
    if (object->steeringFlags & 1) {
      x = object->velocityX;
      z = object->velocityZ;
      if (x != 0 || z != 0) {
        facingStep = (s16)(ArcTan2(z, x) - object->facing);
        if (facingStep > 0x1000)
          facingStep = 0x1000;
        if (facingStep < -0x1000)
          facingStep = -0x1000;
        object->facing += facingStep;
      }
    }
    object = *next++;
  }
}

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

s32 Script_InvokeCallback(struct ScriptInterpreter *interpreter) {
  s16 initial = interpreter->cursor;
  ScriptCommand callback = (ScriptCommand)interpreter->script[initial + 1];

  if (callback(interpreter) != 0)
    return 0;
  if (interpreter->cursor == initial)
    interpreter->cursor += 2;
  return 1;
}

s32 Script_FindLabel(struct ScriptInterpreter *interpreter, u32 key) {
  u32 *entries;
  s32 index;
  s16 *field = &interpreter->lookupResult;
  s32 zero = 0;

  *field = zero;
  if (key == 0) {
    return 0;
  }

  key &= 0xBFFFFFFF;
  entries = (u32 *)interpreter->script;
  index = 0;
  do {
    if (*entries++ == key) {
      return index + 1;
    }
    index++;
  } while (index <= 0x3FF);
  return 0;
}

s32 Script_RepeatOrJump(struct ScriptInterpreter *interpreter) {
  const s32 *arguments;
  s32 repeatLimit;
  s32 jumpKey;
  u8 *repeatCount;
  s32 nextCount;

  arguments = &interpreter->script[interpreter->cursor + 1];
  repeatLimit = *arguments++;
  jumpKey = *arguments;
  if (repeatLimit == 0xFFFF) {
    interpreter->cursor = Script_FindLabel(interpreter, jumpKey);
  } else {
    repeatCount = &interpreter->repeatCount;
    nextCount = *repeatCount + 1;
    *repeatCount = nextCount;
    if ((s32)(u8)nextCount < (s32)(s16)repeatLimit) {
      interpreter->cursor = Script_FindLabel(interpreter, jumpKey);
    } else {
      *repeatCount = 0;
      interpreter->cursor = interpreter->cursor + 3;
    }
  }
  return 1;
}

s32 Script_JumpToLabel(struct ScriptInterpreter *interpreter) {
  interpreter->cursor = Script_FindLabel(
      interpreter, interpreter->script[interpreter->cursor + 1]);
  return 1;
}

u32 Script_JumpIfTrue(struct ScriptInterpreter *interpreter) {
  s32 index = interpreter->cursor;
  const s32 *table = interpreter->script;
  u32 value = table[index + 1];

  if (interpreter->conditionResult != 0) {
    interpreter->cursor = Script_FindLabel(interpreter, value);
  } else {
    interpreter->cursor += 2;
  }
  return 1;
}

u32 Script_JumpIfFalse(struct ScriptInterpreter *interpreter) {
  s32 index = interpreter->cursor;
  const s32 *table = interpreter->script;
  u32 value = table[index + 1];

  if (interpreter->conditionResult == 0) {
    interpreter->cursor = Script_FindLabel(interpreter, value);
  } else {
    interpreter->cursor += 2;
  }
  return 1;
}

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"

s32 Script_End(struct ScriptInterpreter *interpreter) {
  s32 result;

  interpreter->script = gScript_MainScript;
  interpreter->cursor = (result = 0);
  return result;
}

s32 GameFlag_Test(s32);

s32 Script_TestFlag(struct ScriptInterpreter *interpreter) {
  s32 done;

  interpreter->conditionResult =
      GameFlag_Test(interpreter->script[interpreter->cursor + 1]);
  done = 1;
  interpreter->cursor = (u16)interpreter->cursor + 2;
  return done;
}

s32 GameFlag_SetBit(s32);
void GameFlag_ClearBit(s32);

s32 Script_SetFlagAndTest(struct ScriptInterpreter *interpreter) {
  s32 value;

  value = interpreter->script[interpreter->cursor + 1];
  interpreter->conditionResult = GameFlag_Test(value);
  GameFlag_SetBit(value);
  interpreter->cursor += 2;
  return 1;
}

s32 Script_ClearFlagAndTest(struct ScriptInterpreter *interpreter) {
  s32 value;

  value = interpreter->script[interpreter->cursor + 1];
  interpreter->conditionResult = GameFlag_Test(value);
  GameFlag_ClearBit(value);
  interpreter->cursor += 2;
  return 1;
}

s32 Script_ToggleFlag(struct ScriptInterpreter *interpreter) {
  s32 value;

  value = interpreter->script[interpreter->cursor + 1];
  interpreter->conditionResult = GameFlag_Test(value);
  if (interpreter->conditionResult == 1)
    GameFlag_ClearBit(value);
  else
    GameFlag_SetBit(value);
  interpreter->cursor += 2;
  return 1;
}

struct ActorView;
void Actor_SelectAnimation(struct ActorView *object, s32 argument);

s32 Script_SetAnimation(struct ScriptInterpreter *interpreter) {
  Actor_SelectAnimation((struct ActorView *)interpreter,
                        interpreter->script[interpreter->cursor + 1]);
  interpreter->cursor += 2;
  return 1;
}

void Actor_Release();

s32 Actor_ReleaseAndClear(void *object) {
  Actor_Release(object);
  return 0;
}

s32 Script_Nop(struct ScriptInterpreter *interpreter) {
  interpreter->cursor += 2;
  return 1;
}

#include "debug.h"
#include "field_event.h"
#include "edition.h"

s32 Script_PlaySound(struct ScriptInterpreter *interpreter) {
  Sound_PlayFar(interpreter->script[interpreter->cursor + 1]);
  interpreter->cursor += 2;
  return 1;
}

s32 Script_CallWithOperand(struct ScriptInterpreter *interpreter) {
  Func_080c88c8(interpreter, interpreter->script[interpreter->cursor + 1]);
  interpreter->cursor += 2;
  return 1;
}

s32 ScriptObject_CheckOverlap(struct Actor *object, s32 *values) {
  s32 index;
  u8 *flags;
  struct Actor *entry = Ram_HeapSlots->scriptObjects;

  flags = &entry->collisionFlags;
  for (index = 0; index < 64; index++, entry++, flags += sizeof(*entry)) {
    if (entry->script != NULL && (1 & *flags) && entry != object) {
      if (Field_CheckSpheresOverlap(&entry->x, entry->radius - 2, values,
                                    object->radius - 2) >= 0)
        return -1;
    }
  }
  return 0;
}

struct Actor *ScriptObject_FindOverlappingEntry(struct Actor *object,
                                                s32 *values) {
  s32 index;
  u8 *flags;
  struct Actor *entry = Ram_HeapSlots->scriptObjects;

  flags = &entry->collisionFlags;
  for (index = 0; index < 64; index++, entry++, flags += sizeof(*entry)) {
    if (entry->script != NULL && (1 & *flags) && entry != object) {
      if (Field_CheckSpheresOverlap(&entry->x, entry->radius - 2, values,
                                    object->radius - 2) >= 0)
        return entry;
    }
  }
  return NULL;
}
