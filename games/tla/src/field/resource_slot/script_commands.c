/// @file
/// Actor script commands: waits, jumps, flags, moves and work values.
// Part of resource_slot.c.

#include "../resource_slot_internal.h"
#include "flag_call.h"
#include "module/field/field.h"
#include "module/field/resource_slot.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "scene.h"

#define TARGET_UNSET ((s32)0x80000000)

/* VIEW(f02493c): tag struct MotionObject as struct MotionObject__f02493c;
 * resource_slot.c has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/field/resource_slot.c:2499: structure has no member named
 * `terrainHeight'. */
struct MotionObject__f02493c {
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

/* VIEW(f02493c): routine ObjectList_CollectActive as void
 * ObjectList_CollectActive(void *objects, struct MotionObject__f02493c **out);
 * the definition has void ObjectList_CollectActive(u8 *object, u8 **out). */
extern void ObjectList_CollectActive__f02493c(
    void *objects,
    struct MotionObject__f02493c **out) __asm__("ObjectList_CollectActive");

void Actor_UpdateAllMotion(void) {
  struct MotionObject__f02493c *list[15];
  struct MotionObject__f02493c **next;
  struct MotionObject__f02493c *object;
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
  ObjectList_CollectActive__f02493c(object, list);
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

s32 ActorScript_Wait(struct ScriptInterpreter *interpreter) {
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

s32 ActorScript_InvokeCallback(struct ScriptInterpreter *interpreter) {
  s16 initial = interpreter->cursor;
  ScriptCommand callback = (ScriptCommand)interpreter->script[initial + 1];

  if (callback(interpreter) != 0)
    return 0;
  if (interpreter->cursor == initial)
    interpreter->cursor += 2;
  return 1;
}

s32 ActorScript_FindLabel(struct ScriptInterpreter *interpreter, u32 key) {
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

s32 ActorScript_RepeatOrJump(struct ScriptInterpreter *interpreter) {
  const s32 *arguments;
  s32 repeatLimit;
  s32 jumpKey;
  u8 *repeatCount;
  s32 nextCount;

  arguments = &interpreter->script[interpreter->cursor + 1];
  repeatLimit = *arguments++;
  jumpKey = *arguments;
  if (repeatLimit == 0xFFFF) {
    interpreter->cursor = ActorScript_FindLabel(interpreter, jumpKey);
  } else {
    repeatCount = &interpreter->repeatCount;
    nextCount = *repeatCount + 1;
    *repeatCount = nextCount;
    if ((s32)(u8)nextCount < (s32)(s16)repeatLimit) {
      interpreter->cursor = ActorScript_FindLabel(interpreter, jumpKey);
    } else {
      *repeatCount = 0;
      interpreter->cursor = interpreter->cursor + 3;
    }
  }
  return 1;
}

s32 ActorScript_JumpToLabel(struct ScriptInterpreter *interpreter) {
  interpreter->cursor = ActorScript_FindLabel(
      interpreter, interpreter->script[interpreter->cursor + 1]);
  return 1;
}

u32 ActorScript_JumpIfTrue(struct ScriptInterpreter *interpreter) {
  s32 index = interpreter->cursor;
  const s32 *table = interpreter->script;
  u32 value = table[index + 1];

  if (interpreter->conditionResult != 0) {
    interpreter->cursor = ActorScript_FindLabel(interpreter, value);
  } else {
    interpreter->cursor += 2;
  }
  return 1;
}

u32 ActorScript_JumpIfFalse(struct ScriptInterpreter *interpreter) {
  s32 index = interpreter->cursor;
  const s32 *table = interpreter->script;
  u32 value = table[index + 1];

  if (interpreter->conditionResult == 0) {
    interpreter->cursor = ActorScript_FindLabel(interpreter, value);
  } else {
    interpreter->cursor += 2;
  }
  return 1;
}

s32 ActorScript_End(struct ScriptInterpreter *interpreter) {
  s32 result;

  interpreter->script = gScript_MainScript;
  interpreter->cursor = (result = 0);
  return result;
}

s32 ActorScript_TestFlag(struct ScriptInterpreter *interpreter) {
  s32 done;

  interpreter->conditionResult =
      GameFlag_Test(interpreter->script[interpreter->cursor + 1]);
  done = 1;
  interpreter->cursor = (u16)interpreter->cursor + 2;
  return done;
}

/* VIEW(f02493c): routine GameFlag_SetBit as s32 GameFlag_SetBit(s32);
 * flag_call.h has void GameFlag_SetBit(s32 flag). */
extern s32 GameFlag_SetBit__f02493c(s32) __asm__("GameFlag_SetBit");

s32 ActorScript_SetFlagAndTest(struct ScriptInterpreter *interpreter) {
  s32 value;

  value = interpreter->script[interpreter->cursor + 1];
  interpreter->conditionResult = GameFlag_Test(value);
  GameFlag_SetBit__f02493c(value);
  interpreter->cursor += 2;
  return 1;
}

s32 ActorScript_ClearFlagAndTest(struct ScriptInterpreter *interpreter) {
  s32 value;

  value = interpreter->script[interpreter->cursor + 1];
  interpreter->conditionResult = GameFlag_Test(value);
  GameFlag_ClearBit(value);
  interpreter->cursor += 2;
  return 1;
}

s32 ActorScript_ToggleFlagAndTest(struct ScriptInterpreter *interpreter) {
  s32 value;

  value = interpreter->script[interpreter->cursor + 1];
  interpreter->conditionResult = GameFlag_Test(value);
  if (interpreter->conditionResult == 1)
    GameFlag_ClearBit(value);
  else
    GameFlag_SetBit__f02493c(value);
  interpreter->cursor += 2;
  return 1;
}

s32 ActorScript_SetAnimation(struct ScriptInterpreter *interpreter) {
  Actor_SelectAnimation((struct Actor *)interpreter,
                        interpreter->script[interpreter->cursor + 1]);
  interpreter->cursor += 2;
  return 1;
}

void Actor_Release();

s32 Actor_ReleaseAndClear(void *object) {
  Actor_Release(object);
  return 0;
}

s32 ActorScript_Nop(struct ScriptInterpreter *interpreter) {
  interpreter->cursor += 2;
  return 1;
}

#include "field_event.h"

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

s32 ActorScript_CheckOverlap(struct Actor *object, s32 *values) {
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

struct Actor *ActorScript_FindOverlappingEntry(struct Actor *object,
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
#undef TARGET_UNSET

#include "actor_script_view.h"

/* VIEW(apply_relative_position): routine Actor_SetMoveTarget as void
 * Actor_SetMoveTarget(void *, s32, s32, s32); the definition has void
 * Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z). */
extern void Actor_SetMoveTarget__apply_relative_position(
    void *, s32, s32, s32) __asm__("Actor_SetMoveTarget");
/* VIEW(apply_relative_position): routine Actor_SetPositionAndResetMotion as
 * void Actor_SetPositionAndResetMotion(void *, s32, s32, s32); the definition
 * has void Actor_SetPositionAndResetMotion(struct Actor *obj, s32 x, s32 y, s32
 * z). */
extern void Actor_SetPositionAndResetMotion__apply_relative_position(
    void *, s32, s32, s32) __asm__("Actor_SetPositionAndResetMotion");

s32 ActorScript_SetPositionAndResetMotion(struct ActorScriptView *object) {
  const s32 *argument = object->script + (s16)object->scriptCursor + 1;
  s32 x = *argument++;
  s32 y = *argument++;
  s32 z = *argument;
  s32 height;

  Actor_SetPositionAndResetMotion__apply_relative_position(object, x, y, z);
  if (y == -1 && (object->flags & 1)) {
    height = MapGrid_GetTerrainHeight(object->terrainId, object->x, object->z);
    object->y = object->y - object->terrainHeight + height;
    object->terrainHeight = height;
  }
  object->scriptCursor += 4;
  return 1;
}

s32 Func_08025038(struct ActorScriptView *object) {
  s32 height;

  Actor_SetPositionAndResetMotion__apply_relative_position(object, 0, object->y,
                                                           0);
  if (object->flags & 1) {
    height = MapGrid_GetTerrainHeight(object->terrainId, object->x, object->z);
    object->y = object->y - object->terrainHeight + height;
    object->terrainHeight = height;
  }
  object->scriptCursor += 1;
  return 1;
}

/// SC_MOVE(x, y, z): sends the object towards (x, y, z).
s32 ActorScript_ApplyAbsolutePosition(struct ActorScriptView *object) {
  const s32 *argument = object->script + (s16)object->scriptCursor + 1;
  s32 x = *argument++;
  s32 y = *argument++;
  s32 z = *argument;

  Actor_SetMoveTarget__apply_relative_position(object, x, y, z);
  object->scriptCursor += 4;
  return 1;
}

s32 Func_0802509c(struct ActorScriptView *object) {
  const s32 *argument = object->script + (s16)object->scriptCursor + 1;
  s32 x = *argument++;
  s32 z = *argument;

  Actor_SetMoveTarget__apply_relative_position(object, x, object->y, z);
  object->scriptCursor += 3;
  return 1;
}

s32 ActorScript_ApplyRelativePosition(struct ActorScriptView *object) {
  const s32 *cursor = object->script + (s16)object->scriptCursor + 1;
  s32 first = *cursor++;
  s32 second = *cursor++;
  s32 third = *cursor;

  Actor_SetMoveTarget__apply_relative_position(
      object, object->x + first, object->y + second, object->z + third);
  object->scriptCursor += 4;
  return 1;
}

s32 ActorScript_StoreAngleToWatchedActor(struct ActorScriptView *object) {
  struct ActorScriptView *target;

  target = object->watchedObject;
  object->scriptValue = ArcTan2((s32)((u32)target->z - (u32)object->z),
                                (s32)((u32)target->x - (u32)object->x));
  object->scriptCursor++;
  return 1;
}

s32 ActorScript_ApplyWatchedActorPosition(struct ActorScriptView *object) {
  struct ActorScriptView *target;

  target = object->watchedObject;
  Actor_SetMoveTarget__apply_relative_position(object, target->x, target->y,
                                               target->z);
  object->scriptCursor++;
  return 1;
}

s32 ActorScript_ApplyLocalOffsetPosition(struct Actor *object) {
  s32 offset[3];

  Actor_SetMoveTarget(object, object->x + offset[0], object->y + offset[1],
                      object->z + offset[2]);
  object->step += 3;
  return 1;
}

#define TARGET_UNSET ((s32)0x80000000)
#define DISTANCE(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))

s32 FieldCamera_FollowLeaderInBounds(struct Actor *interpreter) {
  struct Actor *cameraObject = interpreter;
  struct Actor *leader;
  struct MapWork *bounds;
  s32 minX;
  s32 minZ;
  s32 maxX;
  s32 maxZ;
  s32 x;
  s32 y;
  s32 z;
  s32 dz;
  s32 dy;
  s32 dx;
  s32 distance;
  s32 moveStep;
  s16 *region;

  bounds = Ram_HeapSlots->mapWork;
  leader = cameraObject->watchedObject;
  minX = bounds->minX + 0x780000;
  minZ = bounds->minZ + leader->y + 0x600000;
  maxX = bounds->maxX - 0x780000;
  maxZ = bounds->maxZ + leader->y - 0x400000;
  cameraObject->motionFlags = 0;
  if (leader != NULL && leader->script != NULL) {
    x = leader->x;
    y = leader->y;
    z = leader->z;
    cameraObject->targetX = TARGET_UNSET;
    cameraObject->targetY = TARGET_UNSET;
    cameraObject->targetZ = TARGET_UNSET;
    if (x < minX)
      x = minX;
    if (z < minZ)
      z = minZ;
    if (x > maxX)
      x = maxX;
    if (z > maxZ)
      z = maxZ;
    if (*(s16 **)bounds->unk0fc != NULL) {
      region = *(s16 **)bounds->unk0fc;
      while (region[0] != -1) {
        minX = (region[0] << 16) - 0x780000;
        minZ = (region[1] << 16) + leader->y - 0x400000;
        maxX = (region[2] << 16) + 0x780000;
        maxZ = (region[3] << 16) + leader->y + 0x600000;
        if (x >= minX && z >= minZ && x <= maxX && z <= maxZ) {
          dx = minX;
          if (DISTANCE(minX, x) > DISTANCE(maxX, x))
            dx = maxX;
          dz = minZ;
          if (DISTANCE(minZ, z) > DISTANCE(maxZ, z))
            dz = maxZ;
          if (DISTANCE(dx, x) <= DISTANCE(dz, z))
            x = dx;
          else
            z = dz;
          break;
        }
        region += 4;
      }
    }
    if (cameraObject->timer != 0) {
      cameraObject->x = x;
      cameraObject->y = y;
      cameraObject->z = z;
    } else {
      dx = (x - cameraObject->x) / 0x10000;
      /* FAKEMATCH(dead): this dy, overwritten below, as in The Broken Seal's
       * twin, which steers register allocation;
       * without it, 129 of 345 instructions differ in tla-en. */
      dy = (y - cameraObject->y) / 0x10000;
      dz = (z - cameraObject->z) / 0x10000;
      distance = Iwram_Sqrt(dx * dx + dz * dz) << 16;
      dx = x - cameraObject->x;
      dy = y - cameraObject->y;
      dz = z - cameraObject->z;
      if (distance < 0x400000)
        distance = FixedSqrt(IwramMulQ16(dx, dx) + IwramMulQ16(dz, dz));
      if ((moveStep = distance / 8) > cameraObject->speedLimit)
        moveStep = cameraObject->speedLimit;
      if (distance < 0x4000) {
        cameraObject->x = x;
        cameraObject->z = z;
      } else {
        if (distance > moveStep) {
          s32 ratio = Iwram_RatioMulQ14(distance, dx);
          s32 (*mulQ16)(s32, s32) = IwramMulQ16;
          dx = mulQ16(ratio, moveStep);
          dz = mulQ16(Iwram_RatioMulQ14(distance, dz), moveStep);
        }
        cameraObject->x += dx;
        cameraObject->z += dz;
      }
      if ((dy < 0 ? -dy : dy) > 0x8000)
        dy /= 4;
      cameraObject->y += dy;
    }
  }
  cameraObject->step++;
  return 1;
}

s32 Func_08025428(struct Actor *cameraObject) {
  struct Actor *leader;
  struct MapWork *bounds;
  s32 minX;
  s32 minZ;
  s32 maxX;
  s32 maxZ;
  s32 x;
  s32 y;
  s32 z;
  s32 dz;
  s32 dy;
  s32 dx;
  s32 distance;
  s32 moveStep;

  bounds = Ram_HeapSlots->mapWork;
  leader = cameraObject->watchedObject;
  minX = bounds->minX + 0x780000;
  minZ = bounds->minZ + 0x600000;
  maxX = bounds->maxX - 0x780000;
  maxZ = bounds->maxZ - 0x400000;
  cameraObject->motionFlags = 0;
  if (leader != NULL && leader->script != NULL) {
    x = leader->x;
    y = leader->y;
    z = leader->z;
    cameraObject->targetX = TARGET_UNSET;
    cameraObject->targetY = TARGET_UNSET;
    cameraObject->targetZ = TARGET_UNSET;
    if (x < minX)
      x = minX;
    if (z < minZ)
      z = minZ;
    if (x > maxX)
      x = maxX;
    if (z > maxZ)
      z = maxZ;
    if (cameraObject->timer != 0) {
      cameraObject->x = x;
      cameraObject->y = y;
      cameraObject->z = z;
    } else {
      dx = (x - cameraObject->x) / Q16_ONE;
      /* FAKEMATCH(dead): this dy, overwritten below, ranks y above the bounds
       * in global allocation and keeps it in r5; without it, 54 of 228
       * instructions differ in tla-en. */
      dy = (y - cameraObject->y) / Q16_ONE;
      dz = (z - cameraObject->z) / Q16_ONE;
      distance = Iwram_Sqrt(dx * dx + dz * dz) << 16;
      dx = x - cameraObject->x;
      dy = y - cameraObject->y;
      dz = z - cameraObject->z;
      if (distance < 0x400000)
        distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
      if ((moveStep = distance / 8) > cameraObject->speedLimit)
        moveStep = cameraObject->speedLimit;
      if (distance < 0x4000) {
        cameraObject->x = x;
        cameraObject->z = z;
      } else {
        if (distance > moveStep) {
          dx = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dx), moveStep);
          dz = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dz), moveStep);
        }
        cameraObject->x += dx;
        cameraObject->z += dz;
      }
      if ((dy < 0 ? -dy : dy) > 0x8000)
        dy /= 4;
      cameraObject->y += dy;
    }
  }
  cameraObject->step++;
  return 1;
}

/// SC_APPROACH_WATCHED: follows the watched object at its speed. More than 16
/// pixels away it walks to 16 pixels short of it (animation 2) and runs on;
/// within 16 it stands (animation 1) and waits for the next frame.
// As ☀️'s function of the same name.
s32 Actor_ApproachWatchedActor(struct Actor *object) {
  s32 distancePixels;
  s32 moveZ;
  s32 dx;
  s32 dz;
  s32 deltaZPixels;
  s32 deltaXPixels;
  s32 remainingDistance;
  s32 moveX;
  struct Actor *watchedObject;

  watchedObject = object->watchedObject;
  object->speedLimit = watchedObject->speedLimit;
  object->acceleration = watchedObject->acceleration;
  dx = watchedObject->x - object->x;
  dz = watchedObject->z - object->z;
  deltaXPixels = dx >> 16;
  deltaZPixels = dz >> 16;
  distancePixels =
      Iwram_Sqrt((deltaXPixels * deltaXPixels) + (deltaZPixels * deltaZPixels));
  if (distancePixels > 0x10) {
    remainingDistance = distancePixels - 0x10;
    moveX = dx * remainingDistance / distancePixels;
    moveZ = dz * remainingDistance / distancePixels;
    Actor_SetMoveTarget(object, object->x + moveX, object->y,
                        object->z + moveZ);
    Actor_SelectAnimation(object, 2);
    object->step = (object->step + 1);
    return 1;
  }
  Actor_SelectAnimation(object, 1);
  return 0;
}

struct ScriptObjectEntry;

/* VIEW(resource_slot): routine Vector_AddPolarOffset as void
 * Vector_AddPolarOffset(s32 radius, s32 angle, struct FieldPosition *position);
 * module/system/system.h has void Vector_AddPolarOffset(s32 radius, s32 angle,
 * s32 *position). */
extern void Vector_AddPolarOffset__resource_slot(
    s32 radius, s32 angle,
    struct FieldPosition *position) __asm__("Vector_AddPolarOffset");
/* VIEW(offset): routine ActorScript_CheckOverlap as s32
 * ActorScript_CheckOverlap(struct ScriptObjectEntry *object, s32 *position);
 * the definition has s32 ActorScript_CheckOverlap(struct Actor *object, s32
 * *values). */
extern s32 ScriptObject_CheckOverlap__offset(
    struct ScriptObjectEntry *object,
    s32 *position) __asm__("ActorScript_CheckOverlap");

/// SC_WANDER(base, range, limit), as The Broken Seal's: walks to a random
/// point base to base + range away within 90 degrees of its heading, that
/// nothing blocks, and within limit pixels of home.
s32 Actor_Wander(struct ActorScriptView *object) {
  struct FieldPosition pos;
  struct FieldPosition probe;
  s32 base;
  const s32 *args;
  s32 range;
  s32 limit;
  s32 radius;
  s32 heading;
  s32 tries;
  s32 dx;
  s32 dz;

  args = &object->script[(s16)object->scriptCursor + 1];
  base = *args++;
  range = *args++;
  limit = *args / 0x10000;
  // The tries are a loop written with goto: as while (++tries <= 7) with
  // continue, 116 of 188 instructions differ in tla-en.
  tries = 0;
  limit = limit * limit;
retry:
  tries++;
  if (tries <= 7) {
    pos.x = object->x;
    pos.y = object->y;
    pos.z = object->z;
    radius = base + Iwram_MulQ16(Random16(), range);
    heading = object->scriptValue + (Random16() >> 2) - (Random16() >> 2);
    Vector_AddPolarOffset__resource_slot(radius, heading, &pos);
    if (ScriptObject_CheckOverlap__offset((struct ScriptObjectEntry *)object,
                                          (s32 *)&pos) != 0)
      goto retry;
    if (MapGrid_CheckCollision((struct Actor *)object, &pos) != 0)
      goto retry;
    radius += 0x80000;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset__resource_slot(radius, heading, &probe);
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset__resource_slot(radius, heading + 0x2000, &probe);
    if (MapGrid_CheckCollision((struct Actor *)object, &probe) != 0)
      goto retry;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset__resource_slot(radius, heading - 0x2000, &probe);
    if (MapGrid_CheckCollision((struct Actor *)object, &probe) != 0)
      goto retry;
    // The offsets in a once-only block keep the division on the loaded x; as
    // plain statements, 16 of 188 instructions differ in tla-en.
    do {
      dx = pos.x / 0x10000 - object->homeX;
      dz = pos.z / 0x10000 - object->homeZ;
    } while (0);
    if (dx * dx + dz * dz > limit)
      goto retry;
    goto found;
  }
  object->scriptValue += 0x8000;
  object->turnedBack = 1;
  return 0;
found:
  {
    /* FAKEMATCH(pin): the object moved to r0 before the target's words load;
     * as a plain local, 2 of 188 instructions differ in tla-en. */
    register struct Actor *moved asm("r0") = (struct Actor *)object;

    /* FAKEMATCH(barrier): the empty asm holding the pin above, which keeps the
     * move ahead of the loads; without it, 2 of 188 instructions differ in
     * tla-en. */
    asm("");
    Actor_SetMoveTarget(moved, pos.x, pos.y, pos.z);
  }
  object->scriptCursor += 4;
  return 1;
}

// The script command that wanders near home.

struct ScriptObjectEntry;

s32 MapGrid_CheckCollision(struct Actor *object,
                           struct FieldPosition *position);

/// SC_WANDER_HOME: moves the object to a random spot within its home radius: up
/// to seven tries at a point a random distance away in a random turn of its
/// heading, each refused if anything overlaps it or blocks the path or the
/// cells beside it. An object outside its home radius (or out of tries) heads
/// back toward home instead. Operands: the minimum distance, the distance range
/// and the home radius.
s32 ActorScript_WanderNearHome(struct ActorScriptView *object) {
  struct FieldPosition candidatePosition;
  struct FieldPosition probePosition;
  s32 minimumDistance;
  const s32 *operands;
  s32 distanceRange;
  s32 homeRadiusSquared;
  s32 travelDistance;
  u16 heading;
  u16 angle;
  s32 attempts;
  s32 dx;
  s32 dz;

  operands = &object->script[(s16)object->scriptCursor + 1];
  minimumDistance = *operands++;
  distanceRange = *operands++;
  homeRadiusSquared = *operands / 0x10000;
  angle = object->scriptValue;
  homeRadiusSquared *= homeRadiusSquared;
  attempts = 0;
  dx = object->x / 0x10000 - object->homeX;
  dz = object->z / 0x10000 - object->homeZ;
  if (dx * dx + dz * dz <= homeRadiusSquared) {
  roam:
    attempts++;
    if (attempts <= 7) {
      travelDistance =
          minimumDistance + Iwram_MulQ16(Random16(), distanceRange);
      heading = angle + (Random16() >> 2) - (Random16() >> 2);
      candidatePosition.x = object->x;
      candidatePosition.y = object->y;
      candidatePosition.z = object->z;
      Vector_AddPolarOffset__resource_slot(0x80000, heading,
                                           &candidatePosition);
      if (ScriptObject_CheckOverlap__offset((struct ScriptObjectEntry *)object,
                                            (s32 *)&candidatePosition) != 0)
        goto roam;
      candidatePosition.x = object->x;
      candidatePosition.y = object->y;
      candidatePosition.z = object->z;
      Vector_AddPolarOffset__resource_slot(travelDistance, heading,
                                           &candidatePosition);
      if (MapGrid_CheckCollision((struct Actor *)object, &candidatePosition) !=
          0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      travelDistance += 0x80000;
      probePosition.z = object->z;
      Vector_AddPolarOffset__resource_slot(travelDistance, heading,
                                           &probePosition);
      if (MapGrid_CheckCollision((struct Actor *)object, &probePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset__resource_slot(travelDistance, heading + 0x2000,
                                           &probePosition);
      if (MapGrid_CheckCollision((struct Actor *)object, &probePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset__resource_slot(travelDistance, heading - 0x2000,
                                           &probePosition);
      if (MapGrid_CheckCollision((struct Actor *)object, &probePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset__resource_slot(travelDistance, heading + 0x4000,
                                           &probePosition);
      if (MapGrid_CheckCollision((struct Actor *)object, &probePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset__resource_slot(travelDistance, heading - 0x4000,
                                           &probePosition);
      if (MapGrid_CheckCollision((struct Actor *)object, &probePosition) != 0)
        goto roam;
      dx = candidatePosition.x / 0x10000 - object->homeX;
      dz = candidatePosition.z / 0x10000 - object->homeZ;
      if (dx * dx + dz * dz > homeRadiusSquared)
        goto roam;
      object->flags59 |= 2;
      Actor_SetMoveTarget((struct Actor *)object, candidatePosition.x,
                          candidatePosition.y, candidatePosition.z);
      goto done;
    }
  }
  attempts = 0;
  angle = (u16)ArcTan2(dz, dx) + 0x8000;
back:
  attempts++;
  if (attempts <= 7) {
    travelDistance = minimumDistance + Iwram_MulQ16(Random16(), distanceRange);
    heading = angle + (Random16() >> 2) - (Random16() >> 2);
    candidatePosition.x = object->x;
    candidatePosition.y = object->y;
    candidatePosition.z = object->z;
    Vector_AddPolarOffset__resource_slot(0x80000, heading, &candidatePosition);
    if (ScriptObject_CheckOverlap__offset((struct ScriptObjectEntry *)object,
                                          (s32 *)&candidatePosition) != 0)
      goto back;
    candidatePosition.x = object->x;
    candidatePosition.y = object->y;
    candidatePosition.z = object->z;
    Vector_AddPolarOffset__resource_slot(travelDistance, heading,
                                         &candidatePosition);
    if (MapGrid_CheckCollision((struct Actor *)object, &candidatePosition) != 0)
      goto back;
    object->flags59 &= ~2;
    Actor_SetMoveTarget((struct Actor *)object, candidatePosition.x,
                        candidatePosition.y, candidatePosition.z);
  }
done:
  object->scriptCursor += 4;
  return 1;
}

void ActorScript_SetOrCompareScriptAddress(struct ScriptOperands *state,
                                           s32 operation, s32 value) {
  s8 result;

  if (operation == 0) {
    state->scriptAddress = value;
    return;
  }
  if (operation == 1) {
    state->scriptAddress += value * 4;
    return;
  }
  result = 0;
  if (state->scriptAddress == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void ActorScript_SetOrCompareCursor(struct ScriptOperands *state, s32 operation,
                                    s32 value) {
  if (operation == 0) {
    state->cursor = value;
  } else if (operation == 1) {
    state->cursor = (s16)state->cursor + value;
  } else {
    state->comparisonResult = (s16)state->cursor == (s16)value;
  }
}

void ActorScript_SetOrCompareFacing(struct ScriptOperands *state, s32 operation,
                                    s32 value) {
  s32 result;

  if (operation == 0) {
    state->unsignedHalfword = value;
  } else if (operation == 1) {
    state->unsignedHalfword = (u16)((u32)state->unsignedHalfword + (u32)value);
  } else {
    u32 current = state->unsignedHalfword;

    result = 0;
    if (current == (u16)value)
      result = 1;
    state->comparisonResult = result;
  }
}

void ActorScript_SetOrCompareX(struct ScriptOperands *state, s32 operation,
                               s32 value) {
  s8 result;

  if (operation == 0) {
    state->word08 = value;
    return;
  }
  if (operation == 1) {
    state->word08 += value;
    return;
  }
  result = 0;
  if (state->word08 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void ActorScript_SetOrCompareY(struct ScriptOperands *state, s32 operation,
                               s32 value) {
  s8 result;

  if (operation == 0) {
    state->word0c = value;
    return;
  }
  if (operation == 1) {
    state->word0c += value;
    return;
  }
  result = 0;
  if (state->word0c == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void ActorScript_SetOrCompareZ(struct ScriptOperands *state, s32 operation,
                               s32 value) {
  s8 result;

  if (operation == 0) {
    state->word10 = value;
    return;
  }
  if (operation == 1) {
    state->word10 += value;
    return;
  }
  result = 0;
  if (state->word10 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void ActorScript_SetOrCompareRadius(struct ScriptOperands *state, s32 operation,
                                    s32 value) {
  s8 result;
  u32 current;

  if (operation == 0) {
    state->halfword20 = value;
    return;
  }
  if (operation == 1) {
    state->halfword20 += value;
    return;
  }
  current = state->halfword20;
  result = 0;
  if (current == (u32)(s16)value)
    result = 1;
  state->comparisonResult = result;
}

void ActorScript_SetOrCompareScaleX(struct ScriptOperands *state, s32 operation,
                                    s32 value) {
  s8 result;

  if (operation == 0) {
    state->word18 = value;
    return;
  }
  if (operation == 1) {
    state->word18 += value;
    return;
  }
  result = 0;
  if (state->word18 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void ActorScript_SetOrCompareScaleY(struct ScriptOperands *state, s32 operation,
                                    s32 value) {
  s8 result;

  if (operation == 0) {
    state->word1c = value;
    return;
  }
  if (operation == 1) {
    state->word1c += value;
    return;
  }
  result = 0;
  if (state->word1c == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void ActorScript_SetOrCompareVelocityX(struct ScriptOperands *state,
                                       s32 operation, s32 value) {
  s8 result;

  if (operation == 0) {
    state->word24 = value;
    return;
  }
  if (operation == 1) {
    state->word24 += value;
    return;
  }
  result = 0;
  if (state->word24 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void ActorScript_SetOrCompareVelocityY(struct ScriptOperands *state,
                                       s32 operation, s32 value) {
  s8 result;

  if (operation == 0) {
    state->word28 = value;
    return;
  }
  if (operation == 1) {
    state->word28 += value;
    return;
  }
  result = 0;
  if (state->word28 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void ActorScript_SetOrCompareVelocityZ(struct ScriptOperands *state,
                                       s32 operation, s32 value) {
  s8 result;

  if (operation == 0) {
    state->word2c = value;
    return;
  }
  if (operation == 1) {
    state->word2c += value;
    return;
  }
  result = 0;
  if (state->word2c == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void ActorScript_SetOrCompareSpeedLimit(struct ScriptOperands *state,
                                        s32 operation, s32 value) {
  s8 result;

  if (operation == 0) {
    state->word30 = value;
    return;
  }
  if (operation == 1) {
    state->word30 += value;
    return;
  }
  result = 0;
  if (state->word30 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void ActorScript_SetOrCompareAcceleration(struct ScriptOperands *state,
                                          s32 operation, s32 value) {
  s8 result;

  if (operation == 0) {
    state->word34 = value;
    return;
  }
  if (operation == 1) {
    state->word34 += value;
    return;
  }
  result = 0;
  if (state->word34 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void ActorScript_SetOrCompareTargetX(struct ScriptOperands *state,
                                     s32 operation, s32 value) {
  s8 result;

  if (operation == 0) {
    state->word38 = value;
    return;
  }
  if (operation == 1) {
    state->word38 += value;
    return;
  }
  result = 0;
  if (state->word38 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void ActorScript_SetOrCompareTargetY(struct ScriptOperands *state,
                                     s32 operation, s32 value) {
  s8 result;

  if (operation == 0) {
    state->word3c = value;
    return;
  }
  if (operation == 1) {
    state->word3c += value;
    return;
  }
  result = 0;
  if (state->word3c == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void ActorScript_SetOrCompareTargetZ(struct ScriptOperands *state,
                                     s32 operation, s32 value) {
  s8 result;

  if (operation == 0) {
    state->word40 = value;
    return;
  }
  if (operation == 1) {
    state->word40 += value;
    return;
  }
  result = 0;
  if (state->word40 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void ActorScript_SetOrCompareVerticalMotion(struct ScriptOperands *state,
                                            s32 operation, s32 value) {
  s8 result;

  if (operation == 0) {
    state->word44 = value;
    return;
  }
  if (operation == 1) {
    state->word44 += value;
    return;
  }
  result = 0;
  if (state->word44 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void ActorScript_SetOrCompareGravity(struct ScriptOperands *state,
                                     s32 operation, s32 value) {
  s8 result;

  if (operation == 0) {
    state->word48 = value;
    return;
  }
  if (operation == 1) {
    state->word48 += value;
    return;
  }
  result = 0;
  if (state->word48 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void ActorScript_SetOrCompareTerrainHeight(struct ScriptOperands *state,
                                           s32 operation, s32 value) {
  s8 result;

  if (operation == 0) {
    state->word14 = value;
    return;
  }
  if (operation == 1) {
    state->word14 += value;
    return;
  }
  result = 0;
  if (state->word14 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void ActorScript_SetOrCompareRiseBaseY(struct ScriptOperands *state,
                                       s32 operation, s32 value) {
  s8 result;

  if (operation == 0) {
    state->word4c = value;
    return;
  }
  if (operation == 1) {
    state->word4c += value;
    return;
  }
  result = 0;
  if (state->word4c == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void ActorScript_SetOrCompareSprite(struct ScriptOperands *state, s32 operation,
                                    s32 value) {
  s8 result;

  if (operation == 0) {
    state->address50 = value;
    return;
  }
  if (operation == 1) {
    state->address50 += value * 4;
    return;
  }
  result = 0;
  if (state->address50 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void ActorScript_SetOrCompareSpriteStorageKind(struct ScriptOperands *state,
                                               s32 operation, s32 value) {
  if (operation == 0) {
    state->byte54 = value;
  } else if (operation == 1) {
    state->byte54 += value;
  } else {
    state->comparisonResult = state->byte54 == (u8)value;
  }
}

void ActorScript_SetOrCompareMotionFlags(struct ScriptOperands *state,
                                         s32 operation, s32 value) {
  if (operation == 0) {
    state->byte55 = value;
  } else if (operation == 1) {
    state->byte55 += value;
  } else {
    state->comparisonResult = state->byte55 == (u8)value;
  }
}

void ActorScript_SetOrCompareArrivalAxis(struct ScriptOperands *state,
                                         s32 operation, s32 value) {
  if (operation == 0) {
    state->byte56 = value;
  } else if (operation == 1) {
    state->byte56 += value;
  } else {
    state->comparisonResult = state->byte56 == (u8)value;
  }
}

void ActorScript_SetOrCompareComparisonResult(struct ScriptOperands *state,
                                              s32 operation, s32 value) {
  s32 result;

  if (operation == 0) {
    state->comparisonResult = value;
  } else if (operation == 1) {
    state->comparisonResult = (u8)((u32)state->comparisonResult + (u32)value);
  } else {
    u8 current = state->comparisonResult;

    result = 0;
    if (current == (u8)value)
      result = 1;
    state->comparisonResult = result;
  }
}

typedef void (*OperandFunc)(struct ScriptOperands *, s32, s32);
extern OperandFunc gActorScript_OperandHandlerTable[];

void ActorScript_SetOrCompareSnapToTarget(struct ScriptOperands *state,
                                          s32 operation, s32 value) {
  if (operation == 0) {
    state->byte58 = value;
  } else if (operation == 1) {
    state->byte58 += value;
  } else {
    state->comparisonResult = state->byte58 == (u8)value;
  }
}

void ActorScript_SetOrCompareCollisionFlags(struct ScriptOperands *state,
                                            s32 operation, s32 value) {
  if (operation == 0) {
    state->byte59 = value;
  } else if (operation == 1) {
    state->byte59 += value;
  } else {
    state->comparisonResult = state->byte59 == (u8)value;
  }
}

void ActorScript_SetOrCompareSteeringFlags(struct ScriptOperands *state,
                                           s32 operation, s32 value) {
  if (operation == 0) {
    state->byte5a = value;
  } else if (operation == 1) {
    state->byte5a += value;
  } else {
    state->comparisonResult = state->byte5a == (u8)value;
  }
}

void ActorScript_SetOrComparePaused(struct ScriptOperands *state, s32 operation,
                                    s32 value) {
  if (operation == 0) {
    state->byte5b = value;
  } else if (operation == 1) {
    state->byte5b += value;
  } else {
    state->comparisonResult = state->byte5b == (u8)value;
  }
}

void ActorScript_SetOrCompareRepeatCount(struct ScriptOperands *work,
                                         s32 operation, s32 value) {
  s32 result;

  if (operation == 0) {
    work->byte5d = value;
  } else if (operation == 1) {
    work->byte5d += value;
  } else {
    result = 0;
    if (work->byte5d == (u8)value)
      result = 1;
    work->comparisonResult = result;
  }
}

void ActorScript_SetOrCompareWait(struct ScriptOperands *work, s32 operation,
                                  s32 value) {
  s8 result;

  if (operation == 0) {
    work->halfword5e = value;
    return;
  }
  if (operation == 1) {
    work->halfword5e += value;
    return;
  }
  result = 0;
  if ((s16)work->halfword5e == (s16)value) {
    result = 1;
  }
  work->comparisonResult = result;
}

void ActorScript_SetOrCompareTimer(struct ScriptOperands *work, s32 operation,
                                   s32 value) {
  if (operation == 0) {
    work->signedHalfword64 = value;
  } else if (operation == 1) {
    work->signedHalfword64 += value;
  } else {
    work->comparisonResult = work->signedHalfword64 == (s16)value;
  }
}

void ActorScript_SetOrCompareUserValue(struct ScriptOperands *work,
                                       s32 operation, s32 value) {
  if (operation == 0) {
    work->signedHalfword66 = value;
  } else if (operation == 1) {
    work->signedHalfword66 += value;
  } else {
    work->comparisonResult = work->signedHalfword66 == (s16)value;
  }
}

void ActorScript_SetOrCompareWatchedActor(struct ScriptOperands *work,
                                          s32 operation, s32 value) {
  s8 result;

  if (operation == 0) {
    work->word68 = value;
    return;
  }
  if (operation == 1) {
    work->word68 += value;
    return;
  }
  result = 0;
  if (work->word68 == value) {
    result = 1;
  }
  work->comparisonResult = result;
}

void ActorScript_SetOrCompareUpdateAddress(struct ScriptOperands *work,
                                           s32 operation, s32 value) {
  s8 result;

  if (operation == 0) {
    work->word6c = value;
    return;
  }
  if (operation == 1) {
    work->word6c += value;
    return;
  }
  result = 0;
  if (work->word6c == value) {
    result = 1;
  }
  work->comparisonResult = result;
}

void ActorScript_SetOrCompareRiseCounter(struct ScriptOperands *work,
                                         s32 operation, s32 value) {
  s32 result;
  if (operation == 0) {
    work->byte62 = value;
  } else if (operation == 1) {
    work->byte62 += value;
  } else {
    result = 0;
    if (work->byte62 == (u8)value)
      result = 1;
    work->comparisonResult = result;
  }
}

void ActorScript_SetOrCompareRiseEnabled(struct ScriptOperands *work,
                                         s32 operation, s32 value) {
  s32 result;
  if (operation == 0) {
    work->byte63 = value;
  } else if (operation == 1) {
    work->byte63 += value;
  } else {
    result = 0;
    if (work->byte63 == (u8)value)
      result = 1;
    work->comparisonResult = result;
  }
}

s32 ActorScript_ApplyOperandSet(struct ScriptOperands *work) {
  s16 index = work->cursor;
  s32 *entry = (s32 *)work->scriptAddress + index + 1;
  OperandFunc callback = gActorScript_OperandHandlerTable[entry[0]];

  if (callback != 0)
    callback(work, 0, entry[1]);
  work->cursor += 3;
  return 1;
}

s32 ActorScript_ApplyOperandAdd(struct ScriptOperands *work) {
  s16 index = work->cursor;
  s32 *entry = (s32 *)work->scriptAddress + index + 1;
  OperandFunc callback = gActorScript_OperandHandlerTable[entry[0]];

  if (callback != 0)
    callback(work, 1, entry[1]);
  work->cursor += 3;
  return 1;
}

s32 ActorScript_ApplyOperandCompare(struct ScriptOperands *work) {
  s16 index = work->cursor;
  s32 *entry = (s32 *)work->scriptAddress + index + 1;
  OperandFunc callback = gActorScript_OperandHandlerTable[entry[0]];

  if (callback != 0)
    callback(work, 2, entry[1]);
  work->cursor += 3;
  return 1;
}

void ScriptObject_SetValue(struct ScriptObject *object, s32 value) {
  if (object != NULL)
    object->value = value;
}

#include "input.h"
#include "frame.h"
#include "event_work.h"

// The heading of each direction-pad combination, -1 for none.
extern const s16 gWorldMap_KeyHeadings[16];

s32 MapGrid_CheckCollisionAbove(struct Actor *object,
                                struct FieldPosition *position);
void *Unit_Get(s32 owner);

s32 Func_0802632c(struct Actor *object) {
  struct FieldPosition position;
  struct FieldPosition testPosition;
  struct FieldPosition contact;
  s16 angleOffsets[22];
  s32 collision;
  s16 heading;
  s32 angle;
  s32 movementAnimation;
  u16 direction;
  s32 angleIndex;
  s32 headingCount;
  struct Actor *other;
  s32 pressed;

  movementAnimation = 2;
  collision = 0;
  object->motionFlags = 4;
  if (gInput.held & ((struct PartyKeys *)&gMainRoutineWork)->dashKeys) {
    object->speedLimit = 0x8000;
    object->acceleration = 0x2000;
    movementAnimation = 5;
  } else {
    object->speedLimit = 0x8000;
    object->acceleration = 0x2000;
  }

  angle = gWorldMap_KeyHeadings[(gInput.held >> 4) & 15];
  if ((u16)angle == 0xffff) {
    collision |= 4;
  } else {
    collision = 0;
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset__resource_slot(0x80000, (u16)angle, &position);
    if (gDebugMode != 0 && (gInput.held & 0x200)) {
      heading = angle;
    } else {
      if (MapGrid_CheckCollisionAbove(object, &position) == 0) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset__resource_slot(0x80000, (u16)angle + 0x1000,
                                             &testPosition);
        if (MapGrid_CheckCollisionAbove(object, &testPosition) == 0) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset__resource_slot(0x80000, (u16)angle - 0x1000,
                                               &testPosition);
          if (MapGrid_CheckCollisionAbove(object, &testPosition) == 0) {
            testPosition.x = object->x;
            testPosition.y = object->y;
            testPosition.z = object->z;
            Vector_AddPolarOffset__resource_slot(0x80000, (u16)angle + 0x2000,
                                                 &testPosition);
            if (MapGrid_CheckCollisionAbove(object, &testPosition) == 0) {
              testPosition.x = object->x;
              testPosition.y = object->y;
              testPosition.z = object->z;
              Vector_AddPolarOffset__resource_slot(0x80000, (u16)angle - 0x2000,
                                                   &testPosition);
              if (MapGrid_CheckCollisionAbove(object, &testPosition) == 0) {
                heading = angle;
                goto check_objects;
              }
            }
          }
        }
      }
      direction = angle;
      headingCount = 0;
      angleOffsets[headingCount++] = direction + 0x1000;
      angleOffsets[headingCount++] = direction - 0x1000;
      angleOffsets[headingCount++] = direction + 0x2000;
      angleOffsets[headingCount++] = direction - 0x2000;
      angleOffsets[headingCount++] = direction + 0x3000;
      angleOffsets[headingCount++] = direction - 0x3000;
      for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
        heading = angleOffsets[angleIndex];
        position.x = object->x;
        position.y = object->y;
        position.z = object->z;
        Vector_AddPolarOffset__resource_slot(0x80000, (u16)heading, &position);
        if (MapGrid_CheckCollisionAbove(object, &position) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset__resource_slot(0x80000, (u16)heading + 0x1000,
                                             &testPosition);
        if (MapGrid_CheckCollisionAbove(object, &testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset__resource_slot(0x80000, (u16)heading - 0x1000,
                                             &testPosition);
        if (MapGrid_CheckCollisionAbove(object, &testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset__resource_slot(0x80000, (u16)heading + 0x2000,
                                             &testPosition);
        if (MapGrid_CheckCollisionAbove(object, &testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset__resource_slot(0x80000, (u16)heading - 0x2000,
                                             &testPosition);
        if (MapGrid_CheckCollisionAbove(object, &testPosition) == 0)
          goto check_objects;
      }
      position.x = object->x;
      position.y = object->y;
      position.z = object->z;
      collision |= 1;

    check_objects:
      contact.x = object->x;
      contact.y = object->y;
      contact.z = object->z;
      Vector_AddPolarOffset__resource_slot(0x40000, (u16)heading, &contact);
      other = Ram_HeapSlots->scriptObjects;
      for (angleIndex = 63; angleIndex >= 0; angleIndex -= 1, other++) {
        s32 radius;

        radius = object->radius - 2;
        if (other->script == 0)
          continue;
        if ((other->collisionFlags & 1) == 0)
          continue;
        if (other == object)
          continue;
        if (Field_CheckSpheresOverlap(&other->x, radius, &contact.x,
                                      other->radius - 2) < 0)
          continue;
        collision |= 2;
      }
    }
  }

  if (Ram_HeapSlots->eventWork != NULL) {
    struct EventWork *work = Ram_HeapSlots->eventWork;

    if (collision & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
  }
  if (collision != 0) {
    s32 action;

    action = 9;
    if (((struct UnitHp *)Unit_Get(gMainRoutineWork.currentOwner))->hp == 0)
      action = 22;
    Actor_SelectAnimation(object, action);
  } else {
    Actor_SelectAnimation(object, movementAnimation);
  }

  if (collision != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    if (collision & 3) {
      s32 difference;

      difference = (u16)angle - object->facing;
      difference = (s16)difference;
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->facing += difference;
    }
    object->timer = 0;
    object->unk66 = 2;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    if (object->timer != 0)
      object->timer--;
  }

  pressed = 0;
  if (Ram_HeapSlots->eventWork == NULL)
    return 0;
  if ((gInput.pressed & gMainRoutineWork.keys234) ||
      (gInput.pressed & gMainRoutineWork.keys230) ||
      (gInput.pressed & gMainRoutineWork.keys236) ||
      (gInput.pressed & gMainRoutineWork.keys238) ||
      (gInput.pressed & gMainRoutineWork.keys23a))
    pressed = 1;
  if (pressed) {
    Sound_PlayFar(113);
  } else {
    object->terrainHeight =
        MapGrid_GetTerrainHeight(object->terrainId, object->x, object->z);
    object->step++;
  }
  return 1;
}
#undef TARGET_UNSET
#undef DISTANCE
