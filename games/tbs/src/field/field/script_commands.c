// Part of field.c, the field object, compiled only through it.
// Object script commands: waits, jumps, flags, moves and work values.

#include "../field_internal.h"

// SC_WAIT(n): waits n frames; this frame counts as the first. Returns 0 to
// end the object's commands for this frame.
s32 Script_Wait(struct ScriptInterpreter *interpreter) {
  s16 commandCursor = interpreter->cursor;

  interpreter->lookupResult = interpreter->script[commandCursor + 1] - 1;
  interpreter->cursor += 2;
  return 0;
}

// SC_SYNC: waits until the object arrives at its target, or until a solid
// object has blocked it for 60 frames (delay is the blocked count, which
// the update raises). Returns 1 (run on) once it has, 0 (wait) until then.
s32 Script_WaitForArrival(struct ScriptInterpreter *interpreter) {
  if (interpreter->delay > 0x3B) {
    interpreter->delay = 0;
    interpreter->cursor++;
    return 1;
  }
  if (Object_IsTargetUnset((struct ObjectRuntime *)interpreter) != 0) {
    interpreter->cursor++;
    return 1;
  }
  return 0;
}

// SC_CALL(fn): calls fn with the object. A non-zero result waits and calls
// it again next frame; zero moves on, past the command unless fn moved the
// cursor itself.
s32 Script_InvokeCallback(struct ScriptInterpreter *interpreter) {
  s16 initialCursor = interpreter->cursor;
  ScriptCommand callback =
      (ScriptCommand)interpreter->script[initialCursor + 1];

  if (callback(interpreter) != 0)
    return 0;
  if (interpreter->cursor == initialCursor)
    interpreter->cursor = interpreter->cursor + 2;
  return 1;
}

// Returns the cursor just past the script's label key (SC_LABEL or SC_TO,
// matched without the bit that tells them apart), searching its first 1024
// words, or 0 (the start) when key is 0 or not found. Clears the wait.
s32 Script_FindLabel(struct ScriptInterpreter *interpreter, u32 labelKey) {
  u32 *scriptWords;
  s32 wordIndex;
  s16 *waitCounter = &interpreter->lookupResult;
  s32 zero = 0;

  *waitCounter = zero;
  if (labelKey == 0) {
    return 0;
  }

  labelKey &= 0xBFFFFFFF;
  scriptWords = (u32 *)interpreter->script;
  wordIndex = 0;
  do {
    if (*scriptWords++ == labelKey) {
      return wordIndex + 1;
    }
    wordIndex++;
  } while (wordIndex <= 0x3FF);
  return 0;
}

// SC_LOOP(n, to): jumps to label to until it has done so n - 1 times, then
// moves on; with n = SC_FOREVER it always jumps.
s32 Script_RepeatOrJump(struct ScriptInterpreter *interpreter) {
  const s32 *arguments;
  s32 repeatLimit;
  s32 jumpKey;
  u8 *repeatCount;
  s32 nextCount;

  arguments = &interpreter->script[interpreter->cursor + 1];
  repeatLimit = *arguments++;
  jumpKey = *arguments;
  if (repeatLimit == SC_FOREVER) {
    interpreter->cursor = Script_FindLabel(interpreter, jumpKey);
  } else {
    repeatCount = &interpreter->repeatCount;
    nextCount = *repeatCount + 1;
    *repeatCount = nextCount;
    if ((u8)nextCount < (s16)repeatLimit) {
      interpreter->cursor = Script_FindLabel(interpreter, jumpKey);
    } else {
      *repeatCount = 0;
      interpreter->cursor += 3;
    }
  }
  return 1;
}

// SC_JUMP(to): jumps to label to.
s32 Script_JumpToLabel(struct ScriptInterpreter *interpreter) {
  interpreter->cursor = Script_FindLabel(
      interpreter, interpreter->script[interpreter->cursor + 1]);
  return 1;
}

// SC_IF(to): jumps to label to when the last test was true.
s32 Script_JumpIfTrue(struct ScriptInterpreter *interpreter) {
  s32 commandCursor = interpreter->cursor;
  const s32 *script = interpreter->script;
  u32 labelKey = script[commandCursor + 1];

  if (interpreter->conditionResult != 0) {
    interpreter->cursor = Script_FindLabel(interpreter, labelKey);
  } else {
    interpreter->cursor += 2;
  }
  return 1;
}

// SC_IFNOT(to): jumps to label to when the last test was false.
s32 Script_JumpIfFalse(struct ScriptInterpreter *interpreter) {
  s32 commandCursor = interpreter->cursor;
  const s32 *script = interpreter->script;
  u32 labelKey = script[commandCursor + 1];

  if (interpreter->conditionResult == 0) {
    interpreter->cursor = Script_FindLabel(interpreter, labelKey);
  } else {
    interpreter->cursor += 2;
  }
  return 1;
}

// SC_END: hands the object back to the idle script. Returns 0.
s32 Script_End(struct ScriptInterpreter *interpreter) {
  s32 result;

  interpreter->script = gScript_MainScript;
  interpreter->cursor = (result = 0);
  return result;
}

// SC_FLAG_CHECK(f): tests game flag f.
s32 Script_TestFlag(struct ScriptInterpreter *interpreter) {
  interpreter->conditionResult =
      GameFlag_TestFar(interpreter->script[interpreter->cursor + 1]);
  interpreter->cursor += 2;
  return 1;
}

// Defines a flag command: stores the flag's current value in conditionResult,
// then changes the flag with change.
#define SCRIPT_DEFINE_FLAG_COMMAND(name, change)                               \
  s32 name(struct ScriptInterpreter *interpreter) {                            \
    s32 flagId;                                                                \
                                                                               \
    flagId = interpreter->script[interpreter->cursor + 1];                     \
    interpreter->conditionResult = GameFlag_TestFar(flagId);                   \
    change(flagId);                                                            \
    interpreter->cursor += 2;                                                  \
    return 1;                                                                  \
  }

// SC_FLAG_ON(f) and SC_FLAG_OFF(f): test game flag f, then set or clear it.
SCRIPT_DEFINE_FLAG_COMMAND(Script_SetFlagAndTest, GameFlag_SetBitFar)
SCRIPT_DEFINE_FLAG_COMMAND(Script_ClearFlagAndTest, GameFlag_ClearBitFar)

// SC_FLAG_REV(f): tests game flag f, then flips it.
s32 Script_ToggleFlagAndTest(struct ScriptInterpreter *interpreter) {
  s32 flagId;
  s32 previousFlagValue;

  flagId = interpreter->script[interpreter->cursor + 1];
  previousFlagValue = GameFlag_TestFar(flagId);
  interpreter->conditionResult = previousFlagValue;
  if ((previousFlagValue << 0x18) == 0x01000000) {
    GameFlag_ClearBitFar(flagId);
  } else {
    GameFlag_SetBitFar(flagId);
  }
  interpreter->cursor += 2;
  return 1;
}

// SC_ARG(v): switches the object's sprites to animation v.
s32 Script_SetAnimation(struct ScriptInterpreter *interpreter) {
  ObjectDispatch_SelectAnimation(interpreter,
                                 interpreter->script[interpreter->cursor + 1]);
  interpreter->cursor += 2;
  return 1;
}

// SC_FREE: releases the object. Returns 0.
s32 Script_FreeObject(struct ScriptInterpreter *interpreter) {
  ObjectDispatch_Release((struct DispatchObject *)interpreter);
  return 0;
}

// SC_NOP(v): does nothing with its operand.
s32 Script_Nop(struct ScriptInterpreter *interpreter) {
  interpreter->cursor += 2;
  return 1;
}

// SC_SE(n): sends sound command n (a sound effect).
s32 Script_PlaySound(struct ScriptInterpreter *interpreter) {
  AudioCommand_PlayFar(interpreter->script[interpreter->cursor + 1]);
  interpreter->cursor += 2;
  return 1;
}

// Returns -1 when an object at position values would overlap a solid
// object (bit 0 of unk59), each radius less 2 pixels; otherwise 0.
s32 ScriptObject_CheckOverlap(struct ScriptObjectEntry *object, s32 *values) {
  s32 objectIndex;
  u8 *collisionFlags;
  struct ObjectRuntime *candidate = gObjectSlots;

  collisionFlags = &candidate->unk59;
  for (objectIndex = 0; objectIndex < 64;
       objectIndex++, candidate++, collisionFlags += sizeof(*candidate)) {
    if (candidate->script != NULL && (1 & *collisionFlags) &&
        candidate != (struct ObjectRuntime *)object) {
      if (Runtime_CheckRadiusOverlap(
              &candidate->x, ((struct FieldActor *)candidate)->radius - 2,
              values, ((struct FieldActor *)object)->radius - 2) >= 0)
        return -1;
    }
  }
  return 0;
}

// Returns the first solid object that an object at position values would
// overlap, as ScriptObject_CheckOverlap tests, or NULL.
struct ScriptObjectEntry *
ScriptObject_FindOverlappingEntry(struct ScriptObjectEntry *object,
                                  s32 *values) {
  s32 objectIndex;
  u8 *collisionFlags;
  struct ObjectRuntime *candidate = gObjectSlots;

  collisionFlags = &candidate->unk59;
  for (objectIndex = 0; objectIndex < 64;
       objectIndex++, candidate++, collisionFlags += sizeof(*candidate)) {
    if (candidate->script != NULL && (1 & *collisionFlags) &&
        candidate != (struct ObjectRuntime *)object) {
      if (Runtime_CheckRadiusOverlap(
              &candidate->x, ((struct FieldActor *)candidate)->radius - 2,
              values, ((struct FieldActor *)object)->radius - 2) >= 0)
        return (struct ScriptObjectEntry *)candidate;
    }
  }
  return NULL;
}

// Reads three coordinate words and applies the selected positioning operation.
static __inline__ s32 Script_ApplyPositionCommand(
    struct ScriptInterpreter *interpreter,
    void (*applyPosition)(struct ObjectRuntime *object, s32 x, s32 y, s32 z)) {
  s32 x;
  s32 y;
  s32 z;
  const s32 *coordinates;

  coordinates = interpreter->script + interpreter->cursor;
  coordinates++;
  x = *coordinates;
  coordinates++;
  y = *coordinates;
  coordinates++;
  z = *coordinates;
  applyPosition((struct ObjectRuntime *)interpreter, x, y, z);
  // The command and its three coordinates occupy four script words.
  interpreter->cursor += 4;
  return 1;
}

// SC_POS(x, y, z): places the object at (x, y, z) and stops it.
s32 Script_SetPositionAndResetMotion(struct ScriptInterpreter *interpreter) {
  return Script_ApplyPositionCommand(interpreter,
                                     Object_SetPositionAndResetMotion);
}

// SC_MOVE(x, y, z): sends the object towards (x, y, z).
s32 Script_ApplyAbsolutePosition(struct ScriptInterpreter *interpreter) {
  return Script_ApplyPositionCommand(interpreter, Object_SetMoveTarget);
}

// SC_MOVE_BY(x, y, z): sends the object towards its position plus (x, y, z).
s32 Script_ApplyRelativePosition(struct ScriptInterpreter *interpreter) {
  struct ScriptObjectRuntime *object =
      (struct ScriptObjectRuntime *)interpreter;

  u8 *commandBytes = (u8 *)(object->script + (s16)object->scriptCursor);
  s32 *arguments = (s32 *)(commandBytes + 4);
  s32 deltaX = *arguments++;
  s32 deltaY = *arguments++;
  s32 deltaZ = *arguments;

  Object_SetMoveTarget((struct ObjectRuntime *)object, object->x + deltaX,
                       object->y + deltaY, object->z + deltaZ);
  object->scriptCursor += 4;
  return 1;
}

// SC_FACE_LINK: stores the angle from the object to its linked object in
// scriptValue.
s32 Script_StoreAngleToLinkedObject(struct ScriptInterpreter *interpreter) {
  struct ScriptObjectRuntime *object =
      (struct ScriptObjectRuntime *)interpreter;

  struct ScriptObjectRuntime *target;

  target = object->linkedObject;
  object->scriptValue =
      ArcTan2((target->z - object->z), (target->x - object->x));
  object->scriptCursor++;
  return 1;
}

// SC_MOVE_LINK: sends the object towards its linked object.
s32 Script_ApplyLinkedObjectPosition(struct ScriptInterpreter *interpreter) {
  struct ScriptObjectRuntime *object =
      (struct ScriptObjectRuntime *)interpreter;

  struct ScriptObjectRuntime *target;

  target = object->linkedObject;
  Object_SetMoveTarget((struct ObjectRuntime *)object, target->x, target->y,
                       target->z);
  object->scriptCursor++;
  return 1;
}

// SC_MOVE_LOCAL(a, b): sends the object towards its position plus an offset
// that is never filled in; the operands are skipped unread.
s32 Script_ApplyLocalOffsetPosition(struct ScriptInterpreter *interpreter) {
  struct ScriptObjectRuntime *object =
      (struct ScriptObjectRuntime *)interpreter;

  s32 localOffset[3];

  // BUG: offset is never set, so the target is the position plus whatever
  // the stack held.
#ifdef UBFIX
  localOffset[0] = localOffset[1] = localOffset[2] = 0;
#endif
  Object_SetMoveTarget((struct ObjectRuntime *)object,
                       object->x + localOffset[0], object->y + localOffset[1],
                       object->z + localOffset[2]);
  object->scriptCursor += 3;
  return 1;
}

// SC_CAMERA: keeps the camera object on its leader (linkedObject), clamped
// to 120 pixels inside the map's left and right edges and a band above and
// below the leader. It closes an eighth of the distance a frame, at most
// its speed limit, and a quarter of a large height difference; when action
// is set it jumps there at once.
s32 Camera_FollowLeaderInBounds(struct ScriptInterpreter *interpreter) {
  struct ObjectRuntime *cameraObject = (struct ObjectRuntime *)interpreter;

  struct ObjectRuntime *leader;
  struct MapScrollWork *bounds;
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

  bounds = gMapWork[0];
  leader = cameraObject->linkedObject;
  minX = bounds->minX + 0x780000;
  minZ = bounds->minY + leader->y + 0x600000;
  maxX = bounds->maxX - 0x780000;
  maxZ = bounds->maxY + leader->y - 0x400000;
  cameraObject->flags = 0;
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
      SET_POSITION(cameraObject, x, y, z);
    } else {
      // The distance in the XZ plane: dy is computed as in
      // Object_SetMoveTarget but not used for the distance.
      dx = (x - cameraObject->x) / 0x10000;
      /* FAKEMATCH(dead): this dy is overwritten below, as in
       * Object_SetMoveTarget's three-axis delta, but its references rank y
       * above the bounds in global allocation and keep it in r5 as native;
       * without it, 52 of 224 instructions differ in tbs-en. */
      dy = (y - cameraObject->y) / 0x10000;
      dz = (z - cameraObject->z) / 0x10000;
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

extern u8 IwramSqrt[];
s32 _call_via_r3(s32, s32, s32, s32);

// SC_APPROACH_LINK: follows the linked object at its speed. More than 16
// pixels away it walks to 16 pixels short of it (animation 2) and runs on;
// within 16 it stands (animation 1) and waits for the next frame.
s32 Object_ApproachLinkedObject(struct ScriptInterpreter *interpreter) {
  struct ObjectRuntime *object = (struct ObjectRuntime *)interpreter;

  s32 distancePixels;
  s32 moveZ;
  s32 dx;
  s32 dz;
  s32 deltaZPixels;
  s32 deltaXPixels;
  s32 remainingDistance;
  s32 moveX;
  s32 scaledDeltaX;
  struct ObjectRuntime *linkedObject;

  linkedObject = object->linkedObject;
  object->speedLimit = linkedObject->speedLimit;
  object->acceleration = linkedObject->acceleration;
  dx = linkedObject->x - object->x;
  dz = linkedObject->z - object->z;
  deltaXPixels = dx >> 0x10;
  deltaZPixels = dz >> 0x10;
  distancePixels = _call_via_r3((deltaXPixels * deltaXPixels) +
                                    (deltaZPixels * deltaZPixels),
                                dx, deltaZPixels, (u32)IwramSqrt);
  if (distancePixels > 0x10) {
    scaledDeltaX = dx;
    remainingDistance = distancePixels - 0x10;
    moveX = scaledDeltaX * remainingDistance / distancePixels;
    moveZ = dz * remainingDistance / distancePixels;
    Object_SetMoveTarget(object, object->x + moveX, object->y,
                         object->z + moveZ);
    ObjectDispatch_SelectAnimation(object, 2);
    object->step = (object->step + 1);
    return 1;
  }
  ObjectDispatch_SelectAnimation(object, 1);
  return 0;
}

// SC_WANDER(base, range, limit): walks to a random point base to base +
// range away, within 90 degrees either side of its heading (scriptValue),
// that no solid object or wall blocks, with clear ground 8 pixels further at
// 45 degrees either side, and within limit pixels of home. After seven
// failed tries it turns around (turnedBack) and waits for the next frame.
s32 Object_Wander(struct ScriptInterpreter *interpreter) {
  struct ScriptObjectRuntime *object =
      (struct ScriptObjectRuntime *)interpreter;

  struct FieldPosition candidatePosition;
  struct FieldPosition probePosition;
  s32 minimumDistance;
  const s32 *operands;
  s32 distanceRange;
  s32 homeRadiusSquared;
  s32 travelDistance;
  s32 heading;
  s32 attempts;
  s32 dx;
  s32 dz;
#ifdef BUGFIX
  s32 aheadBlocked;
#endif

  operands = &object->script[(s16)object->scriptCursor + 1];
  minimumDistance = *operands++;
  distanceRange = *operands++;
  homeRadiusSquared = *operands / 0x10000;
  attempts = 0;
  homeRadiusSquared = homeRadiusSquared * homeRadiusSquared;
retry:
  attempts++;
  if (attempts <= 7) {
    candidatePosition.x = object->x;
    candidatePosition.y = object->y;
    candidatePosition.z = object->z;
    travelDistance = minimumDistance + Iwram_MulQ16(Random16(), distanceRange);
    heading = object->scriptValue + (Random16() >> 2) - (Random16() >> 2);
    Vector_AddPolarOffset(travelDistance, heading, &candidatePosition);
    if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object,
                                  (s32 *)&candidatePosition) != 0 ||
        Map_CheckObjectStep((struct ObjectRuntime *)object,
                            &candidatePosition) != 0)
      goto retry;
    travelDistance += 0x80000;
    probePosition.x = object->x;
    probePosition.y = object->y;
    probePosition.z = object->z;
    Vector_AddPolarOffset(travelDistance, heading, &probePosition);
    // BUG: the probe straight ahead is placed but never tested, so only the
    // two at 45 degrees need clear ground; SC_WANDER_HOME tests all three.
#ifdef BUGFIX
    aheadBlocked =
        Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition);
#endif
    probePosition.x = object->x;
    probePosition.y = object->y;
    probePosition.z = object->z;
    Vector_AddPolarOffset(travelDistance, heading + 0x2000, &probePosition);
#ifdef BUGFIX
    if (aheadBlocked != 0 || Map_CheckObjectStep((struct ObjectRuntime *)object,
                                                 &probePosition) != 0)
#else
    if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition) !=
        0)
#endif
      goto retry;
    probePosition.x = object->x;
    probePosition.y = object->y;
    probePosition.z = object->z;
    Vector_AddPolarOffset(travelDistance, heading - 0x2000, &probePosition);
    if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition) !=
        0)
      goto retry;
    /* FAKEMATCH(one-pass): it ends cse2's block, so the division works on the
     * loaded x and the copy outlives it into the move target call; as plain
     * statements, 8 of 184 instructions differ in tbs-en. The retry gotos stay:
     * as for (;;) with continue, loop.c hoists the two positions' addresses,
     * which the native recomputes each try. */
    do {
      dx = candidatePosition.x / 0x10000 - object->homeX;
      dz = candidatePosition.z / 0x10000 - object->homeZ;
    } while (0);
    if (dx * dx + dz * dz > homeRadiusSquared)
      goto retry;
  } else {
    object->scriptValue += 0x8000;
    object->turnedBack = 1;
    return 0;
  }
  Object_SetMoveTarget((struct ObjectRuntime *)object, candidatePosition.x,
                       candidatePosition.y, candidatePosition.z);
  object->scriptCursor += 4;
  return 1;
}

// SC_WANDER_HOME(base, range, limit): wanders as SC_WANDER does, testing
// more ground around the point, while it is within limit pixels of home;
// outside that, or when no point works, it heads back towards home.
s32 ScriptObject_WanderNearHome(struct ScriptInterpreter *interpreter) {
  struct ScriptObjectRuntime *object =
      (struct ScriptObjectRuntime *)interpreter;

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
      if (Map_CheckObjectStep((struct ObjectRuntime *)object,
                              &candidatePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      travelDistance += 0x80000;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading, &probePosition);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition) !=
          0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading + 0x2000, &probePosition);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition) !=
          0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading - 0x2000, &probePosition);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition) !=
          0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading + 0x4000, &probePosition);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition) !=
          0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading - 0x4000, &probePosition);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition) !=
          0)
        goto roam;
      dx = candidatePosition.x / 0x10000 - object->homeX;
      dz = candidatePosition.z / 0x10000 - object->homeZ;
      if (dx * dx + dz * dz > homeRadiusSquared)
        goto roam;
      object->flags59 |= 2;
      Object_SetMoveTarget((struct ObjectRuntime *)object, candidatePosition.x,
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
    if (Map_CheckObjectStep((struct ObjectRuntime *)object,
                            &candidatePosition) != 0)
      goto back;
    object->flags59 &= ~2;
    Object_SetMoveTarget((struct ObjectRuntime *)object, candidatePosition.x,
                         candidatePosition.y, candidatePosition.z);
  }
done:
  object->scriptCursor += 4;
  return 1;
}

enum ScriptOperandOperation {
  SCRIPT_OPERAND_SET,
  SCRIPT_OPERAND_ADD,
  SCRIPT_OPERAND_COMPARE
};

// Defines the handler for a 32-bit work value: set it, add to it, or compare
// it (into comparisonResult).
#define SCRIPT_DEFINE_OPERAND_WORD(name, field)                                \
  void name(struct ScriptOperands *operands, s32 operation, s32 value) {       \
    if (operation == SCRIPT_OPERAND_SET) {                                     \
      operands->field = value;                                                 \
      return;                                                                  \
    }                                                                          \
    if (operation == SCRIPT_OPERAND_ADD) {                                     \
      operands->field += value;                                                \
      return;                                                                  \
    }                                                                          \
    operands->comparisonResult = operands->field == value;                     \
  }

// The same for an address: an add steps by value words (value * 4 bytes).
#define SCRIPT_DEFINE_OPERAND_ADDRESS(name, field)                             \
  void name(struct ScriptOperands *operands, s32 operation, s32 value) {       \
    if (operation == SCRIPT_OPERAND_SET) {                                     \
      operands->field = value;                                                 \
      return;                                                                  \
    }                                                                          \
    if (operation == SCRIPT_OPERAND_ADD) {                                     \
      operands->field += value * 4;                                            \
      return;                                                                  \
    }                                                                          \
    operands->comparisonResult = operands->field == value;                     \
  }

// The same for an 8- or 16-bit value: a compare narrows value to its type.
#define SCRIPT_DEFINE_OPERAND_NARROW(name, field, type)                        \
  void name(struct ScriptOperands *operands, s32 operation, s32 value) {       \
    if (operation == SCRIPT_OPERAND_SET) {                                     \
      operands->field = value;                                                 \
    } else if (operation == SCRIPT_OPERAND_ADD) {                              \
      operands->field += value;                                                \
    } else {                                                                   \
      operands->comparisonResult = operands->field == (type)value;             \
    }                                                                          \
  }

// The work value handlers, one for each entry of enum ScriptWork (see
// gScript_OperandHandlerTable in operand.c); each names the object field it
// sets, adds to or compares.
SCRIPT_DEFINE_OPERAND_ADDRESS(Script_SetOrCompareAddress, scriptAddress)

// Work value SW_CURSOR: sets, adds to or compares the script cursor.
void Script_SetOrCompareCursor(struct ScriptOperands *operands, s32 operation,
                               s32 value) {
  if (operation == SCRIPT_OPERAND_SET) {
    operands->cursor = value;
  } else if (operation == SCRIPT_OPERAND_ADD) {
    operands->cursor = ((s16)operands->cursor + value);
  } else {
    operands->comparisonResult = (s16)operands->cursor == (s16)value;
  }
}

SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareUnsignedHalfword,
                             unsignedHalfword, u16)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareX, word08)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareY, word0c)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareZ, word10)

// Work value SW_RADIUS: sets, adds to or compares the object's radius.
void Script_SetOrCompareRadius(struct ScriptOperands *state, s32 operation,
                               s32 value) {
  if (operation == SCRIPT_OPERAND_SET) {
    state->halfword20 = value;
    return;
  }
  if (operation == SCRIPT_OPERAND_ADD) {
    state->halfword20 = (state->halfword20 + value);
    return;
  }
  state->comparisonResult = state->halfword20 == (s16)value;
}

SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareScaleX, word18)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareScaleY, word1c)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareVelocityX, word24)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareVelocityY, word28)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareVelocityZ, word2c)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareSpeedLimit, word30)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareAcceleration, word34)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareTargetX, word38)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareTargetY, word3c)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareTargetZ, word40)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareVerticalMotion, word44)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareGravity, word48)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareTerrainHeight, word14)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareWord4c, word4c)
SCRIPT_DEFINE_OPERAND_ADDRESS(Script_SetOrCompareAddress50, address50)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareByte54, byte54, u8)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareMotionFlags, byte55, u8)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareArrivalAxis, byte56, u8)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareComparisonResult,
                             comparisonResult, u8)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareSnapToTarget, byte58, u8)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareCollisionFlags, byte59, u8)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareSteeringFlags, byte5a, u8)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrComparePaused, byte5b, u8)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareRepeatCount, byte5d, u8)

// Work value SW_WAIT: sets, adds to or compares the wait counter.
void Script_SetOrCompareHalfword5e(struct ScriptOperands *work, s32 operation,
                                   s32 value) {
  if (operation == SCRIPT_OPERAND_SET) {
    work->halfword5e = value;
    return;
  }
  if (operation == SCRIPT_OPERAND_ADD) {
    work->halfword5e += value;
    return;
  }
  work->comparisonResult = (s16)work->halfword5e == (s16)value;
}

SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareSignedHalfword64,
                             signedHalfword64, s16)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareSignedHalfword66,
                             signedHalfword66, s16)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareWord68, word68)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareUpdateAddress, word6c)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareByte62, byte62, u8)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareByte63, byte63, u8)

// Reads the two words after the command (a work value number and a value)
// and passes operation to that value's handler.
static __inline__ s32
Script_ApplyOperandCommand(struct ScriptOperands *operands, s32 operation) {
  s16 commandCursor = operands->cursor;
  const s32 *arguments =
      (const s32 *)operands->scriptAddress + commandCursor + 1;
  OperandFunc handler = gScript_OperandHandlerTable[arguments[0]];

  if (handler != 0)
    handler(operands, operation, arguments[1]);
  operands->cursor += 3;
  return 1;
}

// SC_SET(w, v): sets work value w to v.
s32 Script_ApplyOperandSet(struct ScriptInterpreter *interpreter) {
  struct ScriptOperands *operands = (struct ScriptOperands *)interpreter;

  return Script_ApplyOperandCommand(operands, SCRIPT_OPERAND_SET);
}

// SC_ADD(w, v): adds v to work value w.
s32 Script_ApplyOperandAdd(struct ScriptInterpreter *interpreter) {
  struct ScriptOperands *operands = (struct ScriptOperands *)interpreter;

  return Script_ApplyOperandCommand(operands, SCRIPT_OPERAND_ADD);
}

// SC_CMP(w, v): tests whether work value w equals v.
s32 Script_ApplyOperandCompare(struct ScriptInterpreter *interpreter) {
  struct ScriptOperands *operands = (struct ScriptOperands *)interpreter;

  return Script_ApplyOperandCommand(operands, SCRIPT_OPERAND_COMPARE);
}

#undef SCRIPT_DEFINE_FLAG_COMMAND
#undef SCRIPT_DEFINE_OPERAND_WORD
#undef SCRIPT_DEFINE_OPERAND_ADDRESS
#undef SCRIPT_DEFINE_OPERAND_NARROW
