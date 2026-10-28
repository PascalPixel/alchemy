/// @file
/// Actor script commands: waits, jumps, flags, moves and work values.
///
/// Each `SC_` handler here is reached through gActorScript_CommandTable
/// (actor_script.c), which Actor_UpdateAll and Actor_UpdateAllThumb call
/// for each live actor; each returns 1 to run on and 0 to end the actor's
/// frame (the protocol is in this folder's README).

#include "../field_internal.h"

enum {
  SCRIPT_LABEL_SEARCH_WORDS = 1024, ///< script words a jump searches
  SCRIPT_BLOCKED_MOVE_FRAMES = 60,  ///< refused frames SC_SYNC waits through
  SCRIPT_WANDER_TRIES = 7,          ///< random points a wander tries a frame
  /// The wander probes' distance, 8 pixels in 16.16: past the point for
  /// ground, and along the heading for solid actors (SC_WANDER_HOME).
  SCRIPT_WANDER_PROBE_DISTANCE = 8 * Q16_ONE,
  SCRIPT_WANDER_DIAGONAL_ANGLE = 0x2000, ///< one eighth of a turn
  SCRIPT_WANDER_SIDE_ANGLE = 0x4000,     ///< one quarter of a turn
  SCRIPT_HALF_TURN = 0x8000,
  /// Whole pixels SC_APPROACH_LINK stays short of its linked actor.
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
/// past its operand. Returns 0 to end the actor's commands for this frame.
s32 ActorScript_Wait(struct Actor *interpreter) {
  s16 commandCursor = interpreter->scriptCursor;

  interpreter->wait = interpreter->script[commandCursor + 1] - 1;
  interpreter->scriptCursor += 2;
  return 0;
}

/// SC_SYNC: waits until the actor arrives at its target, or until a solid
/// actor has blocked it for 60 frames (refused is the blocked count, which
/// Actor_UpdateAllThumb raises). Returns 1 (run on) once it has, clearing
/// refused when it gave up, and 0 (wait) until then.
s32 ActorScript_WaitForArrival(struct Actor *interpreter) {
  if (interpreter->refused > SCRIPT_BLOCKED_MOVE_FRAMES - 1) {
    interpreter->refused = 0;
    interpreter->scriptCursor++;
    return 1;
  }
  if (Actor_IsTargetUnset(interpreter) != 0) {
    interpreter->scriptCursor++;
    return 1;
  }
  return 0;
}

/// SC_CALL(fn): calls fn with the actor. A non-zero result waits and calls
/// it again next frame; zero moves on, past the command unless fn moved the
/// cursor itself. Events and the field effect scripts of fxdata.c use
/// it to run their own per-actor routines.
s32 ActorScript_InvokeCallback(struct Actor *interpreter) {
  s16 initialCursor = interpreter->scriptCursor;
  ScriptCommand callback =
      (ScriptCommand)interpreter->script[initialCursor + 1];

  if (callback(interpreter) != 0)
    return 0;
  if (interpreter->scriptCursor == initialCursor)
    interpreter->scriptCursor = interpreter->scriptCursor + 2;
  return 1;
}

/// Returns the cursor just past the script's label key (SC_LABEL or SC_TO,
/// matched without the bit that tells them apart), searching its first 1024
/// words, or 0 (the start) when key is 0 or not found, so a missing label
/// restarts the script. Clears the wait.
/// Called by the jumps: SC_JUMP, SC_LOOP, SC_IF and SC_IFNOT.
s32 ActorScript_FindLabel(struct Actor *interpreter, u32 labelKey) {
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
/// nest. The field's own scripts (actor_script.c) end with
/// SC_LOOP(SC_FOREVER, SC_TOP).
s32 ActorScript_RepeatOrJump(struct Actor *interpreter) {
  const s32 *arguments;
  s32 repeatLimit;
  s32 jumpKey;
  u8 *repeatCount;
  s32 nextCount;

  arguments = &interpreter->script[interpreter->scriptCursor + 1];
  repeatLimit = *arguments++;
  jumpKey = *arguments;
  if (repeatLimit == SC_FOREVER) {
    interpreter->scriptCursor = ActorScript_FindLabel(interpreter, jumpKey);
  } else {
    repeatCount = &interpreter->repeatCount;
    nextCount = *repeatCount + 1;
    *repeatCount = nextCount;
    if ((u8)nextCount < (s16)repeatLimit) {
      interpreter->scriptCursor = ActorScript_FindLabel(interpreter, jumpKey);
    } else {
      *repeatCount = 0;
      interpreter->scriptCursor += 3;
    }
  }
  return 1;
}

/// SC_JUMP(to): jumps to label to.
s32 ActorScript_JumpToLabel(struct Actor *interpreter) {
  interpreter->scriptCursor = ActorScript_FindLabel(
      interpreter, interpreter->script[interpreter->scriptCursor + 1]);
  return 1;
}

/// SC_IF(to): jumps to label to when the last test was true.
s32 ActorScript_JumpIfTrue(struct Actor *interpreter) {
  s32 commandCursor = interpreter->scriptCursor;
  const s32 *script = interpreter->script;
  u32 labelKey = script[commandCursor + 1];

  if (interpreter->conditionResult != 0) {
    interpreter->scriptCursor = ActorScript_FindLabel(interpreter, labelKey);
  } else {
    interpreter->scriptCursor += 2;
  }
  return 1;
}

/// SC_IFNOT(to): jumps to label to when the last test was false.
s32 ActorScript_JumpIfFalse(struct Actor *interpreter) {
  s32 commandCursor = interpreter->scriptCursor;
  const s32 *script = interpreter->script;
  u32 labelKey = script[commandCursor + 1];

  if (interpreter->conditionResult == 0) {
    interpreter->scriptCursor = ActorScript_FindLabel(interpreter, labelKey);
  } else {
    interpreter->scriptCursor += 2;
  }
  return 1;
}

/// SC_END: hands the actor back to the idle script, gActorScript_EndScript,
/// whose lone SC_END runs again each frame. Returns 0. The table's unused
/// command numbers from 0x24 also point here.
s32 ActorScript_End(struct Actor *interpreter) {
  s32 result;

  interpreter->script = gActorScript_EndScript;
  interpreter->scriptCursor = (result = 0);
  return result;
}

/// SC_FLAG_CHECK(f): tests game flag f.
s32 ActorScript_TestFlag(struct Actor *interpreter) {
  interpreter->conditionResult =
      GameFlag_GetFar(interpreter->script[interpreter->scriptCursor + 1]);
  interpreter->scriptCursor += 2;
  return 1;
}

/// Defines a flag command: stores the flag's current value in conditionResult,
/// then changes the flag with change.
#define SCRIPT_DEFINE_FLAG_COMMAND(name, change)                               \
  s32 name(struct Actor *interpreter) {                                        \
    s32 flagId;                                                                \
                                                                               \
    flagId = interpreter->script[interpreter->scriptCursor + 1];               \
    interpreter->conditionResult = GameFlag_GetFar(flagId);                    \
    change(flagId);                                                            \
    interpreter->scriptCursor += 2;                                            \
    return 1;                                                                  \
  }

/// SC_FLAG_ON(f) and SC_FLAG_OFF(f): test game flag f, then set or clear it.
SCRIPT_DEFINE_FLAG_COMMAND(ActorScript_SetFlagAndTest, GameFlag_SetFar)
SCRIPT_DEFINE_FLAG_COMMAND(ActorScript_ClearFlagAndTest, GameFlag_ClearFar)

/// SC_FLAG_REV(f): tests game flag f, then flips it.
s32 ActorScript_ToggleFlagAndTest(struct Actor *interpreter) {
  s32 flagId;
  s32 previousFlagValue;

  flagId = interpreter->script[interpreter->scriptCursor + 1];
  previousFlagValue = GameFlag_GetFar(flagId);
  interpreter->conditionResult = previousFlagValue;
  // The test's low byte is 1 when the flag was set.
  if ((previousFlagValue << 0x18) == 0x01000000) {
    GameFlag_ClearFar(flagId);
  } else {
    GameFlag_SetFar(flagId);
  }
  interpreter->scriptCursor += 2;
  return 1;
}

/// SC_ANIMATION(v): switches the actor's sprites to animation v.
s32 ActorScript_SetAnimation(struct Actor *interpreter) {
  Actor_SelectAnimation(interpreter,
                        interpreter->script[interpreter->scriptCursor + 1]);
  interpreter->scriptCursor += 2;
  return 1;
}

/// SC_RELEASE: releases the actor through Actor_Release, which
/// releases its sprites and zeroes its slot. Returns 0: with its script now
/// null, the update loop skips the rest of the actor's frame.
s32 ActorScript_Release(struct Actor *interpreter) {
  Actor_Release(interpreter);
  return 0;
}

/// SC_NOP(v): does nothing with its operand.
s32 ActorScript_Nop(struct Actor *interpreter) {
  interpreter->scriptCursor += 2;
  return 1;
}

/// SC_SE(n): sends sound command n.
s32 ActorScript_PlaySound(struct Actor *interpreter) {
  Sound_PlaySongFar(interpreter->script[interpreter->scriptCursor + 1]);
  interpreter->scriptCursor += 2;
  return 1;
}

/// Returns -1 when an actor at position values would overlap a solid
/// actor (ACTOR_SOLID), each radius less 2 pixels; otherwise 0.
/// The actor itself is skipped. Called by SC_WANDER and SC_WANDER_HOME, the
/// player's control (player_control.c), the motion loop's solid test
/// (actor_motion.c), and the carry and Force field Psynergy through
/// ActorScript_CheckOverlapFar.
s32 ActorScript_CheckOverlap(struct Actor *actor, s32 *values) {
  s32 objectIndex;
  u8 *collisionFlags;
  struct Actor *candidate = gActors;

  collisionFlags = &candidate->collisionFlags;
  for (objectIndex = 0; objectIndex < ACTOR_COUNT;
       objectIndex++, candidate++, collisionFlags += sizeof(*candidate)) {
    if (candidate->script != NULL && (ACTOR_SOLID & *collisionFlags) &&
        candidate != actor) {
      if (Field_CheckSpheresOverlap(&candidate->x.fixed, candidate->radius - 2,
                                    values, actor->radius - 2) >= 0)
        return -1;
    }
  }
  return 0;
}

/// Returns the first solid actor that an actor at position values would
/// overlap, as ActorScript_CheckOverlap tests, or NULL. Called by the Move
/// field Psynergy (move.c) through ActorScript_FindOverlappingEntryFar.
struct Actor *ActorScript_FindOverlappingEntry(struct Actor *actor,
                                               s32 *values) {
  s32 objectIndex;
  u8 *collisionFlags;
  struct Actor *candidate = gActors;

  collisionFlags = &candidate->collisionFlags;
  for (objectIndex = 0; objectIndex < ACTOR_COUNT;
       objectIndex++, candidate++, collisionFlags += sizeof(*candidate)) {
    if (candidate->script != NULL && (ACTOR_SOLID & *collisionFlags) &&
        candidate != actor) {
      if (Field_CheckSpheresOverlap(&candidate->x.fixed, candidate->radius - 2,
                                    values, actor->radius - 2) >= 0)
        return candidate;
    }
  }
  return NULL;
}

/// Reads three coordinate words and applies the selected positioning operation.
static __inline__ s32 ActorScript_ApplyPositionCommand(
    struct Actor *interpreter,
    void (*applyPosition)(struct Actor *actor, s32 x, s32 y, s32 z)) {
  s32 x;
  s32 y;
  s32 z;
  const s32 *coordinates;

  coordinates = interpreter->script + interpreter->scriptCursor;
  coordinates++;
  x = *coordinates;
  coordinates++;
  y = *coordinates;
  coordinates++;
  z = *coordinates;
  applyPosition(interpreter, x, y, z);
  interpreter->scriptCursor += 4;
  return 1;
}

/// SC_POSITION(x, y, z): places the actor at (x, y, z) and stops it.
s32 ActorScript_SetPositionAndResetMotion(struct Actor *interpreter) {
  return ActorScript_ApplyPositionCommand(interpreter,
                                          Actor_SetPositionAndResetMotion);
}

/// SC_MOVE(x, y, z): sends the actor towards (x, y, z).
s32 ActorScript_ApplyAbsolutePosition(struct Actor *interpreter) {
  return ActorScript_ApplyPositionCommand(interpreter, Actor_SetMoveTarget);
}

/// SC_MOVE_BY(x, y, z): sends the actor towards its position plus (x, y, z).
s32 ActorScript_ApplyRelativePosition(struct Actor *interpreter) {
  struct Actor *actor = interpreter;

  u8 *commandBytes = (u8 *)(actor->script + (s16)actor->scriptCursor);
  s32 *arguments = (s32 *)(commandBytes + 4);
  s32 deltaX = *arguments++;
  s32 deltaY = *arguments++;
  s32 deltaZ = *arguments;

  Actor_SetMoveTarget(actor, actor->x.fixed + deltaX, actor->y.fixed + deltaY,
                      actor->z.fixed + deltaZ);
  actor->scriptCursor += 4;
  return 1;
}

/// SC_FACE_LINK: stores the angle from the actor to its linked actor in
/// scriptValue, measured on the ground plane (x and z).
s32 ActorScript_StoreAngleToLinkedActor(struct Actor *interpreter) {
  struct Actor *actor = interpreter;

  struct Actor *target;

  target = actor->linkedActor;
  actor->facing = ArcTan2((target->z.fixed - actor->z.fixed),
                          (target->x.fixed - actor->x.fixed));
  actor->scriptCursor++;
  return 1;
}

/// SC_MOVE_LINK: sends the actor towards its linked actor.
s32 ActorScript_ApplyLinkedActorPosition(struct Actor *interpreter) {
  struct Actor *actor = interpreter;

  struct Actor *target;

  target = actor->linkedActor;
  Actor_SetMoveTarget(actor, target->x.fixed, target->y.fixed, target->z.fixed);
  actor->scriptCursor++;
  return 1;
}

/// SC_MOVE_LOCAL(a, b): sends the actor towards its position plus an offset
/// that is never filled in; the operands are skipped unread.
s32 ActorScript_ApplyLocalOffsetPosition(struct Actor *interpreter) {
  struct Actor *actor = interpreter;

  s32 localOffset[3];

  // BUG: offset is never set, so the target is the position plus whatever
  // the stack held.
#ifdef UBFIX
  localOffset[0] = localOffset[1] = localOffset[2] = 0;
#endif
  Actor_SetMoveTarget(actor, actor->x.fixed + localOffset[0],
                      actor->y.fixed + localOffset[1],
                      actor->z.fixed + localOffset[2]);
  actor->scriptCursor += 3;
  return 1;
}

/// SC_CAMERA: keeps the camera actor on its leader (linkedActor), clamped
/// so the view stays on the map. It closes an eighth of the distance a frame,
/// at most its speed limit, and a quarter of a large height difference; when
/// timer is set it jumps there at once.
///
/// Run each frame by gActor_CameraScript, which
/// Actor_StartCamera starts. Clears the camera's flags and target,
/// moves its position, and counts its step.
s32 FieldCamera_FollowLeaderInBounds(struct Actor *interpreter) {
  struct Actor *cameraActor = interpreter;

  struct Actor *leader;
  struct MapGridScrollWork *bounds;
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

  // Use MapGrid_UpdateLayerScroll's viewport origin and the leader's elevation
  // when clamping its ground position to the viewport bounds.
  bounds = gMapGridWork[0];
  leader = cameraActor->linkedActor;
  // UB: the bounds read the leader's height before the null check below.
#ifdef UBFIX
  if (leader == NULL) {
    cameraActor->motionFlags = 0;
    cameraActor->scriptCursor++;
    return 1;
  }
#endif
  // A layered map draws a point at screen row z - y, so the z bounds move
  // with the leader's height and the clamp keeps that row inside the map.
  minX = bounds->minX + FIELD_VIEW_FOCUS_X;
  minZ = bounds->minY + leader->y.fixed + FIELD_VIEW_FOCUS_Y;
  maxX = bounds->maxX - FIELD_VIEW_FOCUS_X;
  maxZ =
      bounds->maxY + leader->y.fixed - (FIELD_VIEW_HEIGHT - FIELD_VIEW_FOCUS_Y);
  cameraActor->motionFlags = 0;
  if (leader != NULL && leader->script != NULL) {
    x = leader->x.fixed;
    y = leader->y.fixed;
    z = leader->z.fixed;
    cameraActor->targetX = TARGET_UNSET;
    cameraActor->targetY = TARGET_UNSET;
    cameraActor->targetZ = TARGET_UNSET;
    if (x < minX)
      x = minX;
    if (z < minZ)
      z = minZ;
    if (x > maxX)
      x = maxX;
    if (z > maxZ)
      z = maxZ;
    if (cameraActor->timer != 0) {
      SET_ACTOR_POSITION(cameraActor, x, y, z);
    } else {
      // The distance in the XZ plane: dy is computed as in
      // Actor_SetMoveTarget but not used for the distance.
      dx = (x - cameraActor->x.fixed) / Q16_ONE;
      /* FAKEMATCH(dead): this dy, overwritten below, ranks y above the bounds
       * in global allocation and keeps it in r5 as native; without it, 52 of
       * 224 instructions differ in tbs-en. */
      dy = (y - cameraActor->y.fixed) / Q16_ONE;
      dz = (z - cameraActor->z.fixed) / Q16_ONE;
      distance = Iwram_Sqrt(dx * dx + dz * dz) << 16;
      dx = x - cameraActor->x.fixed;
      dy = y - cameraActor->y.fixed;
      dz = z - cameraActor->z.fixed;
      // The whole-pixel root is coarse; under 64 pixels the distance is
      // measured again in 16.16 for a finer result.
      if (distance < CAMERA_FOLLOW_FINE_DISTANCE)
        distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
      if ((moveStep = distance / 8) > cameraActor->speedLimit)
        moveStep = cameraActor->speedLimit;
      // Within a quarter pixel the camera snaps to the point.
      if (distance < CAMERA_FOLLOW_SNAP_DISTANCE) {
        cameraActor->x.fixed = x;
        cameraActor->z.fixed = z;
      } else {
        // Shorten the offset to moveStep along the same direction: dx over
        // the distance is a 16.16 fraction, times the step.
        if (distance > moveStep) {
          dx = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dx), moveStep);
          dz = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dz), moveStep);
        }
        cameraActor->x.fixed += dx;
        cameraActor->z.fixed += dz;
      }
      // Height follows at once within half a pixel, else a quarter a frame.
      if ((dy < 0 ? -dy : dy) > CAMERA_FOLLOW_HEIGHT_SNAP)
        dy /= 4;
      cameraActor->y.fixed += dy;
    }
  }
  cameraActor->scriptCursor++;
  return 1;
}

/// SC_APPROACH_LINK: follows the linked actor at its speed. More than 16
/// pixels away it walks to 16 pixels short of it (animation 2) and runs on;
/// within 16 it stands (animation 1) and waits for the next frame.
///
/// Run each frame by gActor_ApproachLinkScript, which
/// Actor_StartApproachLink starts. Copies the linked actor's speed
/// limit and acceleration, and sets the move target.
s32 Actor_ApproachLinkedActor(struct Actor *interpreter) {
  struct Actor *actor = interpreter;

  s32 distancePixels;
  s32 moveZ;
  s32 dx;
  s32 dz;
  s32 deltaZPixels;
  s32 deltaXPixels;
  s32 remainingDistance;
  s32 moveX;
  s32 scaledDeltaX;
  struct Actor *linkedActor;

  linkedActor = actor->linkedActor;
  actor->speedLimit = linkedActor->speedLimit;
  actor->acceleration = linkedActor->acceleration;
  dx = linkedActor->x.fixed - actor->x.fixed;
  dz = linkedActor->z.fixed - actor->z.fixed;
  deltaXPixels = dx >> 0x10;
  deltaZPixels = dz >> 0x10;
  distancePixels =
      Iwram_Sqrt((deltaXPixels * deltaXPixels) + (deltaZPixels * deltaZPixels));
  // Further than 16 pixels: aim at the point 16 pixels short of the linked
  // actor along the line to it (the 16.16 offset scaled by the whole-pixel
  // ratio) and run on to SC_WAIT(1); otherwise stand and stay on this command.
  if (distancePixels > SCRIPT_APPROACH_DISTANCE) {
    scaledDeltaX = dx;
    remainingDistance = distancePixels - SCRIPT_APPROACH_DISTANCE;
    moveX = scaledDeltaX * remainingDistance / distancePixels;
    moveZ = dz * remainingDistance / distancePixels;
    Actor_SetMoveTarget(actor, actor->x.fixed + moveX, actor->y.fixed,
                        actor->z.fixed + moveZ);
    Actor_SelectAnimation(actor, 2);
    actor->scriptCursor = (actor->scriptCursor + 1);
    return 1;
  }
  Actor_SelectAnimation(actor, 1);
  return 0;
}

/// SC_WANDER(base, range, limit): walks to a random point base to base +
/// range away, within 90 degrees either side of its heading (scriptValue),
/// that no solid actor or wall blocks, with clear ground 8 pixels further at
/// 45 degrees either side, and within limit pixels of home. After seven
/// failed tries it turns around (turnedBack) and waits for the next frame.
/// No script in games/tbs names SC_WANDER; events use SC_WANDER_HOME.
s32 Actor_Wander(struct Actor *interpreter) {
  struct Actor *actor = interpreter;

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
  operands = &actor->script[(s16)actor->scriptCursor + 1];
  minimumDistance = *operands++;
  distanceRange = *operands++;
  homeRadiusSquared = *operands / Q16_ONE;
  attempts = 0;
  homeRadiusSquared = homeRadiusSquared * homeRadiusSquared;
retry:
  attempts++;
  if (attempts <= SCRIPT_WANDER_TRIES) {
    SET_POSITION(&candidatePosition, actor->x.fixed, actor->y.fixed,
                 actor->z.fixed);
    // Random16 is a 16.16 fraction below 1, so the distance is base plus up
    // to range. Two quarter-turn rolls give a heading within 90 degrees
    // either side; their difference falls nearer the heading more often.
    travelDistance = minimumDistance + Iwram_MulQ16(Random16(), distanceRange);
    heading = actor->facing + (Random16() >> 2) - (Random16() >> 2);
    Vector_AddPolarOffset(travelDistance, heading, &candidatePosition);
    if (ActorScript_CheckOverlap(actor, (s32 *)&candidatePosition) != 0 ||
        MapGrid_CheckCollision(actor, &candidatePosition) != COLLISION_NONE)
      goto retry;
    // Probes 8 pixels past the point, ahead and at 45 degrees (0x2000)
    // either side.
    travelDistance += SCRIPT_WANDER_PROBE_DISTANCE;
    SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed,
                 actor->z.fixed);
    Vector_AddPolarOffset(travelDistance, heading, &probePosition);
    // BUG: the probe straight ahead is placed but never tested, so only the
    // two at 45 degrees need clear ground; SC_WANDER_HOME tests all three.
#ifdef BUGFIX
    aheadBlocked = MapGrid_CheckCollision(actor, &probePosition);
#endif
    SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed,
                 actor->z.fixed);
    Vector_AddPolarOffset(
        travelDistance, heading + SCRIPT_WANDER_DIAGONAL_ANGLE, &probePosition);
#ifdef BUGFIX
    if (aheadBlocked != 0 ||
        MapGrid_CheckCollision(actor, &probePosition) != COLLISION_NONE)
#else
    if (MapGrid_CheckCollision(actor, &probePosition) != COLLISION_NONE)
#endif
      goto retry;
    SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed,
                 actor->z.fixed);
    Vector_AddPolarOffset(
        travelDistance, heading - SCRIPT_WANDER_DIAGONAL_ANGLE, &probePosition);
    if (MapGrid_CheckCollision(actor, &probePosition) != COLLISION_NONE)
      goto retry;
    // The retry gotos stay: as for (;;) with continue, loop.c hoists the two
    // positions' addresses, which the native recomputes each try.
    /* FAKEMATCH(one-pass): it ends cse2's block, so the division works on the
     * loaded x; as plain statements, 8 of 184 instructions differ in tbs-en.
     */
    do {
      dx = candidatePosition.x / Q16_ONE - actor->homeX;
      dz = candidatePosition.z / Q16_ONE - actor->homeZ;
    } while (0);
    if (dx * dx + dz * dz > homeRadiusSquared)
      goto retry;
  } else {
    // Out of tries: face the other way (half a turn) and stay on this
    // command, to try again next frame.
    actor->facing += SCRIPT_HALF_TURN;
    actor->wait = 1;
    return 0;
  }
  Actor_SetMoveTarget(actor, candidatePosition.x, candidatePosition.y,
                      candidatePosition.z);
  actor->scriptCursor += 4;
  return 1;
}

/// SC_WANDER_HOME(base, range, limit): wanders as SC_WANDER does, testing
/// more ground around the point, while it is within limit pixels of home;
/// outside that, or when no point works, it heads back towards home.
///
/// Used by scripts in Vault and Vale (vault.c, vale_plaza.c). Sets bit 1 of
/// collisionFlags while it roams and clears it on the way home, sets the move
/// target, and always moves on to the next command.
s32 ActorScript_WanderNearHome(struct Actor *interpreter) {
  struct Actor *actor = interpreter;

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

  operands = &actor->script[(s16)actor->scriptCursor + 1];
  minimumDistance = *operands++;
  distanceRange = *operands++;
  homeRadiusSquared = *operands / Q16_ONE;
  angle = actor->facing;
  homeRadiusSquared *= homeRadiusSquared;
  attempts = 0;
  dx = actor->x.fixed / Q16_ONE - actor->homeX;
  dz = actor->z.fixed / Q16_ONE - actor->homeZ;
  if (dx * dx + dz * dz <= homeRadiusSquared) {
  roam:
    attempts++;
    if (attempts <= SCRIPT_WANDER_TRIES) {
      travelDistance =
          minimumDistance + Iwram_MulQ16(Random16(), distanceRange);
      heading = angle + (Random16() >> 2) - (Random16() >> 2);
      SET_POSITION(&candidatePosition, actor->x.fixed, actor->y.fixed,
                   actor->z.fixed);
      // First no solid actor 8 pixels along the heading, then the point
      // itself, then clear ground 8 pixels past it ahead, at 45 degrees and
      // at 90 degrees either side.
      Vector_AddPolarOffset(SCRIPT_WANDER_PROBE_DISTANCE, heading,
                            &candidatePosition);
      if (ActorScript_CheckOverlap(actor, (s32 *)&candidatePosition) != 0)
        goto roam;
      SET_POSITION(&candidatePosition, actor->x.fixed, actor->y.fixed,
                   actor->z.fixed);
      Vector_AddPolarOffset(travelDistance, heading, &candidatePosition);
      if (MapGrid_CheckCollision(actor, &candidatePosition) != COLLISION_NONE)
        goto roam;
      probePosition.x = actor->x.fixed;
      probePosition.y = actor->y.fixed;
      travelDistance += SCRIPT_WANDER_PROBE_DISTANCE;
      probePosition.z = actor->z.fixed;
      Vector_AddPolarOffset(travelDistance, heading, &probePosition);
      if (MapGrid_CheckCollision(actor, &probePosition) != COLLISION_NONE)
        goto roam;
      SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed,
                   actor->z.fixed);
      Vector_AddPolarOffset(travelDistance,
                            heading + SCRIPT_WANDER_DIAGONAL_ANGLE,
                            &probePosition);
      if (MapGrid_CheckCollision(actor, &probePosition) != COLLISION_NONE)
        goto roam;
      SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed,
                   actor->z.fixed);
      Vector_AddPolarOffset(travelDistance,
                            heading - SCRIPT_WANDER_DIAGONAL_ANGLE,
                            &probePosition);
      if (MapGrid_CheckCollision(actor, &probePosition) != COLLISION_NONE)
        goto roam;
      SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed,
                   actor->z.fixed);
      Vector_AddPolarOffset(travelDistance, heading + SCRIPT_WANDER_SIDE_ANGLE,
                            &probePosition);
      if (MapGrid_CheckCollision(actor, &probePosition) != COLLISION_NONE)
        goto roam;
      SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed,
                   actor->z.fixed);
      Vector_AddPolarOffset(travelDistance, heading - SCRIPT_WANDER_SIDE_ANGLE,
                            &probePosition);
      if (MapGrid_CheckCollision(actor, &probePosition) != COLLISION_NONE)
        goto roam;
      dx = candidatePosition.x / Q16_ONE - actor->homeX;
      dz = candidatePosition.z / Q16_ONE - actor->homeZ;
      if (dx * dx + dz * dz > homeRadiusSquared)
        goto roam;
      actor->collisionFlags |= 2;
      Actor_SetMoveTarget(actor, candidatePosition.x, candidatePosition.y,
                          candidatePosition.z);
      goto done;
    }
  }
  // Head back: aim opposite the angle from home to the actor (dx and dz
  // as measured at the start, or at the last point found too far from home),
  // with the same spread, testing only the near and far points. After seven
  // more failures the actor stays put this time.
  attempts = 0;
  angle = (u16)ArcTan2(dz, dx) + SCRIPT_HALF_TURN;
back:
  attempts++;
  if (attempts <= SCRIPT_WANDER_TRIES) {
    travelDistance = minimumDistance + Iwram_MulQ16(Random16(), distanceRange);
    heading = angle + (Random16() >> 2) - (Random16() >> 2);
    SET_POSITION(&candidatePosition, actor->x.fixed, actor->y.fixed,
                 actor->z.fixed);
    Vector_AddPolarOffset(SCRIPT_WANDER_PROBE_DISTANCE, heading,
                          &candidatePosition);
    if (ActorScript_CheckOverlap(actor, (s32 *)&candidatePosition) != 0)
      goto back;
    SET_POSITION(&candidatePosition, actor->x.fixed, actor->y.fixed,
                 actor->z.fixed);
    Vector_AddPolarOffset(travelDistance, heading, &candidatePosition);
    if (MapGrid_CheckCollision(actor, &candidatePosition) != COLLISION_NONE)
      goto back;
    actor->collisionFlags &= ~2;
    Actor_SetMoveTarget(actor, candidatePosition.x, candidatePosition.y,
                        candidatePosition.z);
  }
done:
  actor->scriptCursor += 4;
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

/// The work value handlers, one for each entry of enum ActorScriptValue (see
/// gActorScript_OperandHandlerTable in operand.c); each names the actor field
/// it sets, adds to or compares.
SCRIPT_DEFINE_OPERAND_ADDRESS(ActorScript_SetOrCompareScriptAddress,
                              scriptAddress)

/// Work value SW_CURSOR: sets, adds to or compares the script cursor.
void ActorScript_SetOrCompareCursor(struct ScriptOperands *operands,
                                    s32 operation, s32 value) {
  if (operation == SCRIPT_OPERAND_SET) {
    operands->cursor = value;
  } else if (operation == SCRIPT_OPERAND_ADD) {
    operands->cursor = ((s16)operands->cursor + value);
  } else {
    operands->conditionResult = (s16)operands->cursor == (s16)value;
  }
}

SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareFacing, facing, u16)
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareX, x)
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareY, y)
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareZ, z)

/// Work value SW_RADIUS: sets, adds to or compares the actor's radius.
void ActorScript_SetOrCompareRadius(struct ScriptOperands *state, s32 operation,
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

SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareScaleX, scaleX)
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareScaleY, scaleY)
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareVelocityX, velocityX)
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareVelocityY, velocityY)
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareVelocityZ, velocityZ)
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareSpeedLimit, speedLimit)
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareAcceleration, acceleration)
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareTargetX, targetX)
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareTargetY, targetY)
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareTargetZ, targetZ)
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareVerticalMotion, bounce)
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareGravity, gravity)
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareTerrainHeight, groundHeight)
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareRiseBaseY, riseBaseY)
SCRIPT_DEFINE_OPERAND_ADDRESS(ActorScript_SetOrCompareSprite, sprite)
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareSpriteStorageKind,
                             spriteStorageKind, u8)
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareMotionFlags, motionFlags,
                             u8)
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareArrivalAxis, arrivalAxis,
                             u8)
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareComparisonResult,
                             conditionResult, u8)
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareSnapToTarget, snapToTarget,
                             u8)
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareCollisionFlags,
                             collisionFlags, u8)
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareSteeringFlags,
                             steeringFlags, u8)
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrComparePaused, paused, u8)
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareRepeatCount, repeatCount,
                             u8)

/// Work value SW_WAIT: sets, adds to or compares the wait counter.
void ActorScript_SetOrCompareWait(struct ScriptOperands *work, s32 operation,
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

SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareTimer, timer, s16)
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareUserValue, userValue, s16)
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareLinkedActor, linkedActor)
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareUpdateAddress, update)
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareRiseCounter, riseCounter,
                             u8)
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareRiseEnabled, riseEnabled,
                             u8)

/// Reads the two words after the command (a work value number and a value)
/// and passes operation to that value's handler. The number is not checked
/// against the table's 41 entries; only a null entry is skipped, and the
/// table has none. Moves past both operands and returns 1.
static __inline__ s32
ActorScript_ApplyOperandCommand(struct ScriptOperands *operands,
                                s32 operation) {
  s16 commandCursor = operands->cursor;
  const s32 *arguments =
      (const s32 *)operands->scriptAddress + commandCursor + 1;
  OperandFunc handler = gActorScript_OperandHandlerTable[arguments[0]];

  if (handler != 0)
    handler(operands, operation, arguments[1]);
  operands->cursor += 3;
  return 1;
}

/// SC_SET(w, v): sets work value w to v.
s32 ActorScript_ApplyOperandSet(struct Actor *interpreter) {
  struct ScriptOperands *operands = (struct ScriptOperands *)interpreter;

  return ActorScript_ApplyOperandCommand(operands, SCRIPT_OPERAND_SET);
}

/// SC_ADD(w, v): adds v to work value w.
s32 ActorScript_ApplyOperandAdd(struct Actor *interpreter) {
  struct ScriptOperands *operands = (struct ScriptOperands *)interpreter;

  return ActorScript_ApplyOperandCommand(operands, SCRIPT_OPERAND_ADD);
}

/// SC_CMP(w, v): tests whether work value w equals v.
s32 ActorScript_ApplyOperandCompare(struct Actor *interpreter) {
  struct ScriptOperands *operands = (struct ScriptOperands *)interpreter;

  return ActorScript_ApplyOperandCommand(operands, SCRIPT_OPERAND_COMPARE);
}

#undef SCRIPT_DEFINE_FLAG_COMMAND
#undef SCRIPT_DEFINE_OPERAND_WORD
#undef SCRIPT_DEFINE_OPERAND_ADDRESS
#undef SCRIPT_DEFINE_OPERAND_NARROW
#undef SCRIPT_LABEL_KEY_MASK
