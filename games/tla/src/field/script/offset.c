#include "script.h"
#include "actor.h"

void Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);

s32 Script_ApplyLocalOffsetPosition(struct Actor *object) {
  s32 offset[3];

  Actor_SetMoveTarget(object, object->x + offset[0], object->y + offset[1],
                      object->z + offset[2]);
  object->step += 3;
  return 1;
}

#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "map.h"
#include "lib.h"

#define TARGET_UNSET ((s32)0x80000000)
#define DISTANCE(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))

s32 IwramMulQ16(s32 a, s32 b) __attribute__((long_call));

s32 Func_08025160(struct Actor *interpreter) {
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
    if (cameraObject->action != 0) {
      cameraObject->x = x;
      cameraObject->y = y;
      cameraObject->z = z;
    } else {
      dx = (x - cameraObject->x) / 0x10000;
      /* FAKEMATCH(dead): this dy, overwritten below, as in The Broken Seal's
       * FieldCamera_FollowLeaderInBounds, which steers register allocation;
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

#include "fixed_math.h"

#define TARGET_UNSET ((s32)0x80000000)

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
    if (cameraObject->action != 0) {
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

void Actor_SelectAnimation(struct Actor *object, s32 argument);

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

#include "field.h"
#include "system.h"
#include "actor_script_view.h"

struct ScriptObjectEntry;

struct FieldPosition {
  s32 x;
  s32 y;
  s32 z;
};

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 ScriptObject_CheckOverlap(struct ScriptObjectEntry *object, s32 *position);

s32 Func_08025698(struct ActorScriptView *object) {
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
    Vector_AddPolarOffset(radius, heading, &pos);
    if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object,
                                  (s32 *)&pos) != 0)
      goto retry;
    if (Func_0802d87c((struct Actor *)object, &pos) != 0)
      goto retry;
    radius += 0x80000;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading, &probe);
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading + 0x2000, &probe);
    if (Func_0802d87c((struct Actor *)object, &probe) != 0)
      goto retry;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading - 0x2000, &probe);
    if (Func_0802d87c((struct Actor *)object, &probe) != 0)
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

s32 Func_0802d87c(struct Actor *object, struct FieldPosition *position);
s32 ArcTan2(s32 y, s32 x);

/// Moves the object to a random spot within its home radius, as The Broken
/// Seal's ActorScript_WanderNearHome does: up to seven tries at a point a
/// random distance away in a random turn of its heading, each refused if
/// anything overlaps it or blocks the path or the cells beside it. An object
/// outside its home radius (or out of tries) heads back toward home instead.
/// Operands: the minimum distance, the distance range and the
/// home radius.
s32 Func_08025830(struct ActorScriptView *object) {
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
      Vector_AddPolarOffset(0x80000, heading, &candidatePosition);
      if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object,
                                    (s32 *)&candidatePosition) != 0)
        goto roam;
      candidatePosition.x = object->x;
      candidatePosition.y = object->y;
      candidatePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading, &candidatePosition);
      if (Func_0802d87c((struct Actor *)object, &candidatePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      travelDistance += 0x80000;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading, &probePosition);
      if (Func_0802d87c((struct Actor *)object, &probePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading + 0x2000, &probePosition);
      if (Func_0802d87c((struct Actor *)object, &probePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading - 0x2000, &probePosition);
      if (Func_0802d87c((struct Actor *)object, &probePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading + 0x4000, &probePosition);
      if (Func_0802d87c((struct Actor *)object, &probePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading - 0x4000, &probePosition);
      if (Func_0802d87c((struct Actor *)object, &probePosition) != 0)
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
    Vector_AddPolarOffset(0x80000, heading, &candidatePosition);
    if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object,
                                  (s32 *)&candidatePosition) != 0)
      goto back;
    candidatePosition.x = object->x;
    candidatePosition.y = object->y;
    candidatePosition.z = object->z;
    Vector_AddPolarOffset(travelDistance, heading, &candidatePosition);
    if (Func_0802d87c((struct Actor *)object, &candidatePosition) != 0)
      goto back;
    object->flags59 &= ~2;
    Actor_SetMoveTarget((struct Actor *)object, candidatePosition.x,
                        candidatePosition.y, candidatePosition.z);
  }
done:
  object->scriptCursor += 4;
  return 1;
}

void Script_SetOrCompareAddress(struct ScriptOperands *state, s32 operation,
                                s32 value) {
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

void Script_SetOrCompareCursor(struct ScriptOperands *state, s32 operation,
                               s32 value) {
  if (operation == 0) {
    state->cursor = value;
  } else if (operation == 1) {
    state->cursor = (s16)state->cursor + value;
  } else {
    state->comparisonResult = (s16)state->cursor == (s16)value;
  }
}

void Script_SetOrCompareUnsignedHalfword(struct ScriptOperands *state,
                                         s32 operation, s32 value) {
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

void Script_SetOrCompareX(struct ScriptOperands *state, s32 operation,
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

void Script_SetOrCompareY(struct ScriptOperands *state, s32 operation,
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

void Script_SetOrCompareZ(struct ScriptOperands *state, s32 operation,
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

void Script_SetOrCompareRadius(struct ScriptOperands *state, s32 operation,
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

void Script_SetOrCompareScaleX(struct ScriptOperands *state, s32 operation,
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

void Script_SetOrCompareScaleY(struct ScriptOperands *state, s32 operation,
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

void Script_SetOrCompareVelocityX(struct ScriptOperands *state, s32 operation,
                                  s32 value) {
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

void Script_SetOrCompareVelocityY(struct ScriptOperands *state, s32 operation,
                                  s32 value) {
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

void Script_SetOrCompareVelocityZ(struct ScriptOperands *state, s32 operation,
                                  s32 value) {
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

void Script_SetOrCompareSpeedLimit(struct ScriptOperands *state, s32 operation,
                                   s32 value) {
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

void Script_SetOrCompareAcceleration(struct ScriptOperands *state,
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

void Script_SetOrCompareTargetX(struct ScriptOperands *state, s32 operation,
                                s32 value) {
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

void Script_SetOrCompareTargetY(struct ScriptOperands *state, s32 operation,
                                s32 value) {
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

void Script_SetOrCompareTargetZ(struct ScriptOperands *state, s32 operation,
                                s32 value) {
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

void Script_SetOrCompareVerticalMotion(struct ScriptOperands *state,
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

void Script_SetOrCompareGravity(struct ScriptOperands *state, s32 operation,
                                s32 value) {
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

void Script_SetOrCompareTerrainHeight(struct ScriptOperands *state,
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

void Script_SetOrCompareWord4c(struct ScriptOperands *state, s32 operation,
                               s32 value) {
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

void Script_SetOrCompareAddress50(struct ScriptOperands *state, s32 operation,
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

void Script_SetOrCompareByte54(struct ScriptOperands *state, s32 operation,
                               s32 value) {
  if (operation == 0) {
    state->byte54 = value;
  } else if (operation == 1) {
    state->byte54 += value;
  } else {
    state->comparisonResult = state->byte54 == (u8)value;
  }
}

void Script_SetOrCompareMotionFlags(struct ScriptOperands *state, s32 operation,
                                    s32 value) {
  if (operation == 0) {
    state->byte55 = value;
  } else if (operation == 1) {
    state->byte55 += value;
  } else {
    state->comparisonResult = state->byte55 == (u8)value;
  }
}

void Script_SetOrCompareArrivalAxis(struct ScriptOperands *state, s32 operation,
                                    s32 value) {
  if (operation == 0) {
    state->byte56 = value;
  } else if (operation == 1) {
    state->byte56 += value;
  } else {
    state->comparisonResult = state->byte56 == (u8)value;
  }
}

void Script_SetOrCompareComparisonResult(struct ScriptOperands *state,
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
extern OperandFunc gScript_OperandHandlerTable[];

void Script_SetOrCompareSnapToTarget(struct ScriptOperands *state,
                                     s32 operation, s32 value) {
  if (operation == 0) {
    state->byte58 = value;
  } else if (operation == 1) {
    state->byte58 += value;
  } else {
    state->comparisonResult = state->byte58 == (u8)value;
  }
}

void Script_SetOrCompareCollisionFlags(struct ScriptOperands *state,
                                       s32 operation, s32 value) {
  if (operation == 0) {
    state->byte59 = value;
  } else if (operation == 1) {
    state->byte59 += value;
  } else {
    state->comparisonResult = state->byte59 == (u8)value;
  }
}

void Script_SetOrCompareSteeringFlags(struct ScriptOperands *state,
                                      s32 operation, s32 value) {
  if (operation == 0) {
    state->byte5a = value;
  } else if (operation == 1) {
    state->byte5a += value;
  } else {
    state->comparisonResult = state->byte5a == (u8)value;
  }
}

void Script_SetOrComparePaused(struct ScriptOperands *state, s32 operation,
                               s32 value) {
  if (operation == 0) {
    state->byte5b = value;
  } else if (operation == 1) {
    state->byte5b += value;
  } else {
    state->comparisonResult = state->byte5b == (u8)value;
  }
}

void Script_SetOrCompareRepeatCount(struct ScriptOperands *work, s32 operation,
                                    s32 value) {
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

void Script_SetOrCompareHalfword5e(struct ScriptOperands *work, s32 operation,
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

void Script_SetOrCompareSignedHalfword64(struct ScriptOperands *work,
                                         s32 operation, s32 value) {
  if (operation == 0) {
    work->signedHalfword64 = value;
  } else if (operation == 1) {
    work->signedHalfword64 += value;
  } else {
    work->comparisonResult = work->signedHalfword64 == (s16)value;
  }
}

void Script_SetOrCompareSignedHalfword66(struct ScriptOperands *work,
                                         s32 operation, s32 value) {
  if (operation == 0) {
    work->signedHalfword66 = value;
  } else if (operation == 1) {
    work->signedHalfword66 += value;
  } else {
    work->comparisonResult = work->signedHalfword66 == (s16)value;
  }
}

void Script_SetOrCompareWord68(struct ScriptOperands *work, s32 operation,
                               s32 value) {
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

void Script_SetOrCompareUpdateAddress(struct ScriptOperands *work,
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

void Script_SetOrCompareByte62(struct ScriptOperands *work, s32 operation,
                               s32 value) {
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

void Script_SetOrCompareByte63(struct ScriptOperands *work, s32 operation,
                               s32 value) {
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

s32 Script_ApplyOperandSet(struct ScriptOperands *work) {
  s16 index = work->cursor;
  s32 *entry = (s32 *)work->scriptAddress + index + 1;
  OperandFunc callback = gScript_OperandHandlerTable[entry[0]];

  if (callback != 0)
    callback(work, 0, entry[1]);
  work->cursor += 3;
  return 1;
}

s32 Script_ApplyOperandAdd(struct ScriptOperands *work) {
  s16 index = work->cursor;
  s32 *entry = (s32 *)work->scriptAddress + index + 1;
  OperandFunc callback = gScript_OperandHandlerTable[entry[0]];

  if (callback != 0)
    callback(work, 1, entry[1]);
  work->cursor += 3;
  return 1;
}

s32 Script_ApplyOperandCompare(struct ScriptOperands *work) {
  s16 index = work->cursor;
  s32 *entry = (s32 *)work->scriptAddress + index + 1;
  OperandFunc callback = gScript_OperandHandlerTable[entry[0]];

  if (callback != 0)
    callback(work, 2, entry[1]);
  work->cursor += 3;
  return 1;
}

struct ScriptObject {
  u8 unk00[0x6c];
  s32 value;
};

void ScriptObject_SetValue(struct ScriptObject *object, s32 value) {
  if (object != NULL)
    object->value = value;
}

#include "input.h"
#include "frame.h"
#include "event_work.h"
#include "main_routine_work.h"

struct PartyKeys {
  u8 unk000[0x23c];
  u16 dashKeys;
};

struct UnitHp {
  u8 unk00[0x38];
  s16 hp;
};

// The heading of each direction-pad combination, -1 for none.
extern const s16 gData_0802ec5c[16];

s32 Func_0802d958(struct Actor *object, struct FieldPosition *position);
void *Unit_Get(s32 owner);
s32 Func_0802d45c(s32 terrainId, s32 x, s32 z);

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

  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  if ((u16)angle == 0xffff) {
    collision |= 4;
  } else {
    collision = 0;
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)angle, &position);
    if (gDebugMode != 0 && (gInput.held & 0x200)) {
      heading = angle;
    } else {
      if (Func_0802d958(object, &position) == 0) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)angle + 0x1000, &testPosition);
        if (Func_0802d958(object, &testPosition) == 0) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, (u16)angle - 0x1000, &testPosition);
          if (Func_0802d958(object, &testPosition) == 0) {
            testPosition.x = object->x;
            testPosition.y = object->y;
            testPosition.z = object->z;
            Vector_AddPolarOffset(0x80000, (u16)angle + 0x2000, &testPosition);
            if (Func_0802d958(object, &testPosition) == 0) {
              testPosition.x = object->x;
              testPosition.y = object->y;
              testPosition.z = object->z;
              Vector_AddPolarOffset(0x80000, (u16)angle - 0x2000,
                                    &testPosition);
              if (Func_0802d958(object, &testPosition) == 0) {
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
        Vector_AddPolarOffset(0x80000, (u16)heading, &position);
        if (Func_0802d958(object, &position) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading + 0x1000, &testPosition);
        if (Func_0802d958(object, &testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading - 0x1000, &testPosition);
        if (Func_0802d958(object, &testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading + 0x2000, &testPosition);
        if (Func_0802d958(object, &testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading - 0x2000, &testPosition);
        if (Func_0802d958(object, &testPosition) == 0)
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
      Vector_AddPolarOffset(0x40000, (u16)heading, &contact);
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

      difference = (u16)angle - object->angle;
      difference = (s16)difference;
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->angle += difference;
    }
    object->action = 0;
    object->unk66 = 2;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    if (object->action != 0)
      object->action--;
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
        Func_0802d45c(object->terrainId, object->x, object->z);
    object->step++;
  }
  return 1;
}
