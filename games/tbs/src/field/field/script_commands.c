/// @file
/// Object script commands: waits, jumps, flags, moves and work values.
///
/// Each `SC_` handler here is reached through gScriptObject_CommandTable
/// (object_script.c), which Object_UpdateAll and Object_UpdateAllThumb call
/// for each live object; each returns 1 to run on and 0 to end the object's
/// frame (the protocol is in this folder's README).

#include "../field_internal.h"

enum {
  SCRIPT_LABEL_SEARCH_WORDS = 1024, ///< script words a jump searches
  SCRIPT_BLOCKED_MOVE_FRAMES = 60,  ///< refused frames SC_SYNC waits through
  SCRIPT_WANDER_TRIES = 7,          ///< random points a wander tries a frame
  /// The wander probes' distance, 8 pixels in 16.16: past the point for
  /// ground, and along the heading for solid objects (SC_WANDER_HOME).
  SCRIPT_WANDER_PROBE_DISTANCE = 8 * Q16_ONE,
  SCRIPT_WANDER_DIAGONAL_ANGLE = 0x2000, ///< one eighth of a turn
  SCRIPT_WANDER_SIDE_ANGLE = 0x4000,     ///< one quarter of a turn
  SCRIPT_HALF_TURN = 0x8000,
  /// Whole pixels SC_APPROACH_LINK stays short of its linked object.
  SCRIPT_APPROACH_DISTANCE = 16,
  /// Under 64 pixels (16.16) SC_CAMERA measures its distance in 16.16.
  CAMERA_FOLLOW_FINE_DISTANCE = 64 * Q16_ONE,
  /// Within a quarter pixel (16.16) SC_CAMERA lands on its point.
  CAMERA_FOLLOW_SNAP_DISTANCE = Q16_ONE / 4,
  /// Within half a pixel (16.16) SC_CAMERA takes the leader's height at once.
  CAMERA_FOLLOW_HEIGHT_SNAP = Q16_ONE / 2
};

/// SC_LABEL and SC_TO differ only in bit 30; a jump searches for the label.
#define SCRIPT_LABEL_KEY_MASK 0xbfffffff

/// SC_WAIT(n): waits n frames; this frame counts as the first. Sets
/// interpreter->wait to n - 1, which the update loops count down, and moves
/// past its operand. Returns 0 to end the object's commands for this frame.
s32 Script_Wait(struct ScriptInterpreter *interpreter) {
  s16 commandCursor = interpreter->cursor;

  interpreter->wait = interpreter->script[commandCursor + 1] - 1;
  interpreter->cursor += 2;
  return 0;
}

/// SC_SYNC: waits until the object arrives at its target, or until a solid
/// object has blocked it for 60 frames (refused is the blocked count, which
/// Object_UpdateAllThumb raises). Returns 1 (run on) once it has, clearing
/// refused when it gave up, and 0 (wait) until then.
s32 Script_WaitForArrival(struct ScriptInterpreter *interpreter) {
  if (interpreter->refused > SCRIPT_BLOCKED_MOVE_FRAMES - 1) {
    interpreter->refused = 0;
    interpreter->cursor++;
    return 1;
  }
  if (Object_IsTargetUnset((struct ObjectRuntime *)interpreter) != 0) {
    interpreter->cursor++;
    return 1;
  }
  return 0;
}

/// SC_CALL(fn): calls fn with the object. A non-zero result waits and calls
/// it again next frame; zero moves on, past the command unless fn moved the
/// cursor itself. Scene scripts and the field effect scripts of fxdata.c use
/// it to run their own per-object steps.
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

/// Returns the cursor just past the script's label key (SC_LABEL or SC_TO,
/// matched without the bit that tells them apart), searching its first 1024
/// words, or 0 (the start) when key is 0 or not found, so a missing label
/// restarts the script. Clears the wait.
/// Called by the jumps: SC_JUMP, SC_LOOP, SC_IF and SC_IFNOT.
s32 Script_FindLabel(struct ScriptInterpreter *interpreter, u32 labelKey) {
  u32 *scriptWords;
  s32 wordIndex;
  s16 *waitCounter = &interpreter->wait;
  s32 zero = 0;

  *waitCounter = zero;
  if (labelKey == 0) {
    return 0;
  }

  labelKey &= SCRIPT_LABEL_KEY_MASK;
  scriptWords = (u32 *)interpreter->script;
  wordIndex = 0;
  do {
    if (*scriptWords++ == labelKey) {
      return wordIndex + 1;
    }
    wordIndex++;
  } while (wordIndex <= SCRIPT_LABEL_SEARCH_WORDS - 1);
  return 0;
}

/// SC_LOOP(n, to): jumps to label to until it has done so n - 1 times, then
/// moves on; with n = SC_FOREVER it always jumps. Counts in
/// interpreter->repeatCount and clears it when the loop ends, so loops do not
/// nest. The engine's own scripts (object_script.c) end with
/// SC_LOOP(SC_FOREVER, SC_TOP).
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

/// SC_JUMP(to): jumps to label to.
s32 Script_JumpToLabel(struct ScriptInterpreter *interpreter) {
  interpreter->cursor = Script_FindLabel(
      interpreter, interpreter->script[interpreter->cursor + 1]);
  return 1;
}

/// SC_IF(to): jumps to label to when the last test was true.
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

/// SC_IFNOT(to): jumps to label to when the last test was false.
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

/// SC_END: hands the object back to the idle script, gScript_MainScript,
/// whose lone SC_END runs again each frame. Returns 0. The table's unused
/// command numbers from 0x24 also point here.
s32 Script_End(struct ScriptInterpreter *interpreter) {
  s32 result;

  interpreter->script = gScript_MainScript;
  interpreter->cursor = (result = 0);
  return result;
}

/// SC_FLAG_CHECK(f): tests game flag f.
s32 Script_TestFlag(struct ScriptInterpreter *interpreter) {
  interpreter->conditionResult =
      GameFlag_TestFar(interpreter->script[interpreter->cursor + 1]);
  interpreter->cursor += 2;
  return 1;
}

/// Defines a flag command: stores the flag's current value in conditionResult,
/// then changes the flag with change.
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

/// SC_FLAG_ON(f) and SC_FLAG_OFF(f): test game flag f, then set or clear it.
SCRIPT_DEFINE_FLAG_COMMAND(Script_SetFlagAndTest, GameFlag_SetBitFar)
SCRIPT_DEFINE_FLAG_COMMAND(Script_ClearFlagAndTest, GameFlag_ClearBitFar)

/// SC_FLAG_REV(f): tests game flag f, then flips it.
s32 Script_ToggleFlagAndTest(struct ScriptInterpreter *interpreter) {
  s32 flagId;
  s32 previousFlagValue;

  flagId = interpreter->script[interpreter->cursor + 1];
  previousFlagValue = GameFlag_TestFar(flagId);
  interpreter->conditionResult = previousFlagValue;
  // The test's low byte is 1 when the flag was set.
  if ((previousFlagValue << 0x18) == 0x01000000) {
    GameFlag_ClearBitFar(flagId);
  } else {
    GameFlag_SetBitFar(flagId);
  }
  interpreter->cursor += 2;
  return 1;
}

/// SC_ARG(v): switches the object's sprites to animation v.
s32 Script_SetAnimation(struct ScriptInterpreter *interpreter) {
  ObjectDispatch_SelectAnimation(interpreter,
                                 interpreter->script[interpreter->cursor + 1]);
  interpreter->cursor += 2;
  return 1;
}

/// SC_FREE: releases the object through ObjectDispatch_Release, which
/// releases its sprites and zeroes its slot. Returns 0: with its script now
/// null, the update loop skips the rest of the object's frame.
s32 Script_FreeObject(struct ScriptInterpreter *interpreter) {
  ObjectDispatch_Release((struct DispatchObject *)interpreter);
  return 0;
}

/// SC_NOP(v): does nothing with its operand.
s32 Script_Nop(struct ScriptInterpreter *interpreter) {
  interpreter->cursor += 2;
  return 1;
}

/// SC_SE(n): sends sound command n (a sound effect).
s32 Script_PlaySound(struct ScriptInterpreter *interpreter) {
  AudioCommand_PlayFar(interpreter->script[interpreter->cursor + 1]);
  interpreter->cursor += 2;
  return 1;
}

/// Returns -1 when an object at position values would overlap a solid
/// object (bit 0 of collisionFlags), each radius less 2 pixels; otherwise 0.
/// The object itself is skipped. Called by SC_WANDER and SC_WANDER_HOME, the
/// player's control (player_control.c), the motion loop's solid test
/// (object_motion.c), and the carry and Force effects through
/// ScriptObject_CheckOverlapFar.
s32 ScriptObject_CheckOverlap(struct ScriptObjectEntry *object, s32 *values) {
  s32 objectIndex;
  u8 *collisionFlags;
  struct ObjectRuntime *candidate = gObjectSlots;

  collisionFlags = &candidate->collisionFlags;
  for (objectIndex = 0; objectIndex < FIELD_OBJECT_COUNT;
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

/// Returns the first solid object that an object at position values would
/// overlap, as ScriptObject_CheckOverlap tests, or NULL. Called by the Move
/// effect (move.c) through ScriptObject_FindOverlappingEntryFar.
struct ScriptObjectEntry *
ScriptObject_FindOverlappingEntry(struct ScriptObjectEntry *object,
                                  s32 *values) {
  s32 objectIndex;
  u8 *collisionFlags;
  struct ObjectRuntime *candidate = gObjectSlots;

  collisionFlags = &candidate->collisionFlags;
  for (objectIndex = 0; objectIndex < FIELD_OBJECT_COUNT;
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

/// Reads three coordinate words and applies the selected positioning operation.
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
  interpreter->cursor += 4;
  return 1;
}

/// SC_POS(x, y, z): places the object at (x, y, z) and stops it.
s32 Script_SetPositionAndResetMotion(struct ScriptInterpreter *interpreter) {
  return Script_ApplyPositionCommand(interpreter,
                                     Object_SetPositionAndResetMotion);
}

/// SC_MOVE(x, y, z): sends the object towards (x, y, z).
s32 Script_ApplyAbsolutePosition(struct ScriptInterpreter *interpreter) {
  return Script_ApplyPositionCommand(interpreter, Object_SetMoveTarget);
}

/// SC_MOVE_BY(x, y, z): sends the object towards its position plus (x, y, z).
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

/// SC_FACE_LINK: stores the angle from the object to its linked object in
/// scriptValue, measured on the ground plane (x and z).
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

/// SC_MOVE_LINK: sends the object towards its linked object.
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

/// SC_MOVE_LOCAL(a, b): sends the object towards its position plus an offset
/// that is never filled in; the operands are skipped unread.
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

/// SC_CAMERA: keeps the camera object on its leader (linkedObject), clamped
/// so the view stays on the map. It closes an eighth of the distance a frame,
/// at most its speed limit, and a quarter of a large height difference; when
/// action is set it jumps there at once.
///
/// Run each frame by gObjectDispatch_CameraScript, which
/// ObjectDispatch_StartCamera starts. Clears the camera's flags and target,
/// moves its position, and counts its step.
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

  // Use Map_UpdateLayerScroll's viewport origin and the leader's elevation
  // when clamping its ground position to the viewport bounds.
  bounds = gMapWork[0];
  leader = cameraObject->linkedObject;
  // UB: the bounds read the leader's height before the null check below.
#ifdef UBFIX
  if (leader == NULL) {
    cameraObject->flags = 0;
    cameraObject->step++;
    return 1;
  }
#endif
  // A layered map draws a point at screen row z - y, so the z bounds move
  // with the leader's height and the clamp keeps that row inside the map.
  minX = bounds->minX + FIELD_VIEW_FOCUS_X;
  minZ = bounds->minY + leader->y + FIELD_VIEW_FOCUS_Y;
  maxX = bounds->maxX - FIELD_VIEW_FOCUS_X;
  maxZ = bounds->maxY + leader->y - (FIELD_VIEW_HEIGHT - FIELD_VIEW_FOCUS_Y);
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
      /* FAKEMATCH(dead): this dy, overwritten below, ranks y above the bounds
       * in global allocation and keeps it in r5 as native; without it, 52 of
       * 224 instructions differ in tbs-en. */
      dy = (y - cameraObject->y) / 0x10000;
      dz = (z - cameraObject->z) / 0x10000;
      distance = Iwram_Sqrt(dx * dx + dz * dz) << 16;
      dx = x - cameraObject->x;
      dy = y - cameraObject->y;
      dz = z - cameraObject->z;
      // The whole-pixel root is coarse; under 64 pixels the distance is
      // measured again in 16.16 for a finer result.
      if (distance < CAMERA_FOLLOW_FINE_DISTANCE)
        distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
      if ((moveStep = distance / 8) > cameraObject->speedLimit)
        moveStep = cameraObject->speedLimit;
      // Within a quarter pixel the camera snaps to the point.
      if (distance < CAMERA_FOLLOW_SNAP_DISTANCE) {
        cameraObject->x = x;
        cameraObject->z = z;
      } else {
        // Shorten the offset to moveStep along the same direction: dx over
        // the distance is a 16.16 fraction, times the step.
        if (distance > moveStep) {
          dx = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dx), moveStep);
          dz = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dz), moveStep);
        }
        cameraObject->x += dx;
        cameraObject->z += dz;
      }
      // Height follows at once within half a pixel, else a quarter a frame.
      if ((dy < 0 ? -dy : dy) > CAMERA_FOLLOW_HEIGHT_SNAP)
        dy /= 4;
      cameraObject->y += dy;
    }
  }
  cameraObject->step++;
  return 1;
}

extern u8 IwramSqrt[];
s32 _call_via_r3(s32, s32, s32, s32);

/// SC_APPROACH_LINK: follows the linked object at its speed. More than 16
/// pixels away it walks to 16 pixels short of it (animation 2) and runs on;
/// within 16 it stands (animation 1) and waits for the next frame.
///
/// Run each frame by gObjectDispatch_ApproachLinkScript, which
/// ObjectDispatch_StartApproachLink starts. Copies the linked object's speed
/// limit and acceleration, and sets the move target.
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
  // The IWRAM square root, called by address through r3; it reads only the
  // first argument, and dx and deltaZPixels fill r1 and r2.
  // XXX: why r1 and r2 carry dx and deltaZPixels; whether the routine is
  // known to take more than one argument would settle it.
  distancePixels = _call_via_r3((deltaXPixels * deltaXPixels) +
                                    (deltaZPixels * deltaZPixels),
                                dx, deltaZPixels, (u32)IwramSqrt);
  // Further than 16 pixels: aim at the point 16 pixels short of the linked
  // object along the line to it (the 16.16 offset scaled by the whole-pixel
  // ratio) and run on to SC_WAIT(1); otherwise stand and stay on this command.
  if (distancePixels > SCRIPT_APPROACH_DISTANCE) {
    scaledDeltaX = dx;
    remainingDistance = distancePixels - SCRIPT_APPROACH_DISTANCE;
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

/// SC_WANDER(base, range, limit): walks to a random point base to base +
/// range away, within 90 degrees either side of its heading (scriptValue),
/// that no solid object or wall blocks, with clear ground 8 pixels further at
/// 45 degrees either side, and within limit pixels of home. After seven
/// failed tries it turns around (turnedBack) and waits for the next frame.
/// No script in games/tbs names SC_WANDER; scenes use SC_WANDER_HOME.
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

  // base and range are 16.16 pixels; limit is compared in whole pixels,
  // against homeX and homeZ.
  operands = &object->script[(s16)object->scriptCursor + 1];
  minimumDistance = *operands++;
  distanceRange = *operands++;
  homeRadiusSquared = *operands / 0x10000;
  attempts = 0;
  homeRadiusSquared = homeRadiusSquared * homeRadiusSquared;
retry:
  attempts++;
  if (attempts <= SCRIPT_WANDER_TRIES) {
    SET_POSITION(&candidatePosition, object->x, object->y, object->z);
    // Random16 is a 16.16 fraction below 1, so the distance is base plus up
    // to range. Two quarter-turn rolls give a heading within 90 degrees
    // either side; their difference falls nearer the heading more often.
    travelDistance = minimumDistance + Iwram_MulQ16(Random16(), distanceRange);
    heading = object->scriptValue + (Random16() >> 2) - (Random16() >> 2);
    Vector_AddPolarOffset(travelDistance, heading, &candidatePosition);
    if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object,
                                  (s32 *)&candidatePosition) != 0 ||
        Map_CheckObjectStep((struct ObjectRuntime *)object,
                            &candidatePosition) != 0)
      goto retry;
    // Probes 8 pixels past the point, ahead and at 45 degrees (0x2000)
    // either side.
    travelDistance += SCRIPT_WANDER_PROBE_DISTANCE;
    SET_POSITION(&probePosition, object->x, object->y, object->z);
    Vector_AddPolarOffset(travelDistance, heading, &probePosition);
    // BUG: the probe straight ahead is placed but never tested, so only the
    // two at 45 degrees need clear ground; SC_WANDER_HOME tests all three.
#ifdef BUGFIX
    aheadBlocked =
        Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition);
#endif
    SET_POSITION(&probePosition, object->x, object->y, object->z);
    Vector_AddPolarOffset(
        travelDistance, heading + SCRIPT_WANDER_DIAGONAL_ANGLE, &probePosition);
#ifdef BUGFIX
    if (aheadBlocked != 0 || Map_CheckObjectStep((struct ObjectRuntime *)object,
                                                 &probePosition) != 0)
#else
    if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition) !=
        0)
#endif
      goto retry;
    SET_POSITION(&probePosition, object->x, object->y, object->z);
    Vector_AddPolarOffset(
        travelDistance, heading - SCRIPT_WANDER_DIAGONAL_ANGLE, &probePosition);
    if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition) !=
        0)
      goto retry;
    // The retry gotos stay: as for (;;) with continue, loop.c hoists the two
    // positions' addresses, which the native recomputes each try.
    /* FAKEMATCH(one-pass): it ends cse2's block, so the division works on the
     * loaded x; as plain statements, 8 of 184 instructions differ in tbs-en.
     */
    do {
      dx = candidatePosition.x / 0x10000 - object->homeX;
      dz = candidatePosition.z / 0x10000 - object->homeZ;
    } while (0);
    if (dx * dx + dz * dz > homeRadiusSquared)
      goto retry;
  } else {
    // Out of tries: face the other way (half a turn) and stay on this
    // command, to try again next frame.
    object->scriptValue += SCRIPT_HALF_TURN;
    object->turnedBack = 1;
    return 0;
  }
  Object_SetMoveTarget((struct ObjectRuntime *)object, candidatePosition.x,
                       candidatePosition.y, candidatePosition.z);
  object->scriptCursor += 4;
  return 1;
}

/// SC_WANDER_HOME(base, range, limit): wanders as SC_WANDER does, testing
/// more ground around the point, while it is within limit pixels of home;
/// outside that, or when no point works, it heads back towards home.
///
/// Used by scripts in Vault and Vale (vault.c, vale_home.c). Sets bit 1 of
/// flags59 while it roams and clears it on the way home, sets the move
/// target, and always moves on to the next command.
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
    if (attempts <= SCRIPT_WANDER_TRIES) {
      travelDistance =
          minimumDistance + Iwram_MulQ16(Random16(), distanceRange);
      heading = angle + (Random16() >> 2) - (Random16() >> 2);
      SET_POSITION(&candidatePosition, object->x, object->y, object->z);
      // First no solid object 8 pixels along the heading, then the point
      // itself, then clear ground 8 pixels past it ahead, at 45 degrees and
      // at 90 degrees either side.
      Vector_AddPolarOffset(SCRIPT_WANDER_PROBE_DISTANCE, heading,
                            &candidatePosition);
      if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object,
                                    (s32 *)&candidatePosition) != 0)
        goto roam;
      SET_POSITION(&candidatePosition, object->x, object->y, object->z);
      Vector_AddPolarOffset(travelDistance, heading, &candidatePosition);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object,
                              &candidatePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      travelDistance += SCRIPT_WANDER_PROBE_DISTANCE;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading, &probePosition);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition) !=
          0)
        goto roam;
      SET_POSITION(&probePosition, object->x, object->y, object->z);
      Vector_AddPolarOffset(travelDistance,
                            heading + SCRIPT_WANDER_DIAGONAL_ANGLE,
                            &probePosition);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition) !=
          0)
        goto roam;
      SET_POSITION(&probePosition, object->x, object->y, object->z);
      Vector_AddPolarOffset(travelDistance,
                            heading - SCRIPT_WANDER_DIAGONAL_ANGLE,
                            &probePosition);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition) !=
          0)
        goto roam;
      SET_POSITION(&probePosition, object->x, object->y, object->z);
      Vector_AddPolarOffset(travelDistance, heading + SCRIPT_WANDER_SIDE_ANGLE,
                            &probePosition);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition) !=
          0)
        goto roam;
      SET_POSITION(&probePosition, object->x, object->y, object->z);
      Vector_AddPolarOffset(travelDistance, heading - SCRIPT_WANDER_SIDE_ANGLE,
                            &probePosition);
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
  // Head back: aim opposite the angle from home to the object (dx and dz
  // as measured at the start, or at the last point found too far from home),
  // with the same spread, testing only the near and far points. After seven
  // more failures the object stays put this time.
  attempts = 0;
  angle = (u16)ArcTan2(dz, dx) + SCRIPT_HALF_TURN;
back:
  attempts++;
  if (attempts <= SCRIPT_WANDER_TRIES) {
    travelDistance = minimumDistance + Iwram_MulQ16(Random16(), distanceRange);
    heading = angle + (Random16() >> 2) - (Random16() >> 2);
    SET_POSITION(&candidatePosition, object->x, object->y, object->z);
    Vector_AddPolarOffset(SCRIPT_WANDER_PROBE_DISTANCE, heading,
                          &candidatePosition);
    if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object,
                                  (s32 *)&candidatePosition) != 0)
      goto back;
    SET_POSITION(&candidatePosition, object->x, object->y, object->z);
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

/// What a work value handler does with its value, for SC_SET, SC_ADD and
/// SC_CMP in turn.
enum ScriptOperandOperation {
  SCRIPT_OPERAND_SET,
  SCRIPT_OPERAND_ADD,
  SCRIPT_OPERAND_COMPARE ///< sets conditionResult to 1 when equal, else 0
};

/// Defines the handler for a 32-bit work value: set it, add to it, or compare
/// it (into conditionResult).
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
    operands->conditionResult = operands->field == value;                      \
  }

/// The same for an address: an add steps by value words (value * 4 bytes).
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
    operands->conditionResult = operands->field == value;                      \
  }

/// The same for an 8- or 16-bit value: a compare narrows value to its type.
#define SCRIPT_DEFINE_OPERAND_NARROW(name, field, type)                        \
  void name(struct ScriptOperands *operands, s32 operation, s32 value) {       \
    if (operation == SCRIPT_OPERAND_SET) {                                     \
      operands->field = value;                                                 \
    } else if (operation == SCRIPT_OPERAND_ADD) {                              \
      operands->field += value;                                                \
    } else {                                                                   \
      operands->conditionResult = operands->field == (type)value;              \
    }                                                                          \
  }

/// The work value handlers, one for each entry of enum ScriptWork (see
/// gScript_OperandHandlerTable in operand.c); each names the object field it
/// sets, adds to or compares.
SCRIPT_DEFINE_OPERAND_ADDRESS(Script_SetOrCompareAddress, scriptAddress)

/// Work value SW_CURSOR: sets, adds to or compares the script cursor.
void Script_SetOrCompareCursor(struct ScriptOperands *operands, s32 operation,
                               s32 value) {
  if (operation == SCRIPT_OPERAND_SET) {
    operands->cursor = value;
  } else if (operation == SCRIPT_OPERAND_ADD) {
    operands->cursor = ((s16)operands->cursor + value);
  } else {
    operands->conditionResult = (s16)operands->cursor == (s16)value;
  }
}

SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareUnsignedHalfword, facing, u16)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareX, x)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareY, y)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareZ, z)

/// Work value SW_RADIUS: sets, adds to or compares the object's radius.
void Script_SetOrCompareRadius(struct ScriptOperands *state, s32 operation,
                               s32 value) {
  if (operation == SCRIPT_OPERAND_SET) {
    state->radius = value;
    return;
  }
  if (operation == SCRIPT_OPERAND_ADD) {
    state->radius = (state->radius + value);
    return;
  }
  state->conditionResult = state->radius == (s16)value;
}

SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareScaleX, scaleX)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareScaleY, scaleY)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareVelocityX, velocityX)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareVelocityY, velocityY)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareVelocityZ, velocityZ)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareSpeedLimit, speedLimit)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareAcceleration, acceleration)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareTargetX, targetX)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareTargetY, targetY)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareTargetZ, targetZ)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareVerticalMotion, bounce)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareGravity, gravity)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareTerrainHeight, terrainHeight)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareWord4c, sceneWord)
SCRIPT_DEFINE_OPERAND_ADDRESS(Script_SetOrCompareAddress50, sprite)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareByte54, spriteStorageKind, u8)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareMotionFlags, motionFlags, u8)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareArrivalAxis, arrivalAxis, u8)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareComparisonResult,
                             conditionResult, u8)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareSnapToTarget, snapToTarget, u8)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareCollisionFlags, collisionFlags,
                             u8)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareSteeringFlags, steeringFlags,
                             u8)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrComparePaused, paused, u8)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareRepeatCount, repeatCount, u8)

/// Work value SW_WAIT: sets, adds to or compares the wait counter.
void Script_SetOrCompareHalfword5e(struct ScriptOperands *work, s32 operation,
                                   s32 value) {
  if (operation == SCRIPT_OPERAND_SET) {
    work->wait = value;
    return;
  }
  if (operation == SCRIPT_OPERAND_ADD) {
    work->wait += value;
    return;
  }
  work->conditionResult = (s16)work->wait == (s16)value;
}

SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareSignedHalfword64,
                             actionParameter, s16)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareSignedHalfword66, footprintSide,
                             s16)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareWord68, linkedObject)
SCRIPT_DEFINE_OPERAND_WORD(Script_SetOrCompareUpdateAddress, hook)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareByte62, sceneByte62, u8)
SCRIPT_DEFINE_OPERAND_NARROW(Script_SetOrCompareByte63, sceneByte63, u8)

/// Reads the two words after the command (a work value number and a value)
/// and passes operation to that value's handler. The number is not checked
/// against the table's 41 entries; only a null entry is skipped, and the
/// table has none. Moves past both operands and returns 1.
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

/// SC_SET(w, v): sets work value w to v.
s32 Script_ApplyOperandSet(struct ScriptInterpreter *interpreter) {
  struct ScriptOperands *operands = (struct ScriptOperands *)interpreter;

  return Script_ApplyOperandCommand(operands, SCRIPT_OPERAND_SET);
}

/// SC_ADD(w, v): adds v to work value w.
s32 Script_ApplyOperandAdd(struct ScriptInterpreter *interpreter) {
  struct ScriptOperands *operands = (struct ScriptOperands *)interpreter;

  return Script_ApplyOperandCommand(operands, SCRIPT_OPERAND_ADD);
}

/// SC_CMP(w, v): tests whether work value w equals v.
s32 Script_ApplyOperandCompare(struct ScriptInterpreter *interpreter) {
  struct ScriptOperands *operands = (struct ScriptOperands *)interpreter;

  return Script_ApplyOperandCommand(operands, SCRIPT_OPERAND_COMPARE);
}

#undef SCRIPT_DEFINE_FLAG_COMMAND
#undef SCRIPT_DEFINE_OPERAND_WORD
#undef SCRIPT_DEFINE_OPERAND_ADDRESS
#undef SCRIPT_DEFINE_OPERAND_NARROW
#undef SCRIPT_LABEL_KEY_MASK
