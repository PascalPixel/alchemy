/// @file
/// Actor script commands: waits, jumps, flags, moves and state values.
///
/// Each `SC_` handler here is reached through gActorScript_CommandTable
/// (actor_script.c), which Actor_UpdateAll and Actor_UpdateAllThumb call
/// for each live actor; each returns 1 to run on and 0 to end the actor's
/// frame (the protocol is in this folder's README).

#include "field_internal.h"

/// The interpreter's limits, distances and turns.
enum {
  SCRIPT_LABEL_SEARCH_WORDS = 1024, ///< script words a jump searches
  SCRIPT_BLOCKED_MOVE_FRAMES = 60,  ///< refused frames SC_SYNC waits through
  SCRIPT_WANDER_TRIES = 7,          ///< random points a wander tries a frame
  /// The wander probes' distance, 8 pixels in 16.16: past the point for
  /// ground, and along the heading for solid actors (SC_WANDER_HOME).
  SCRIPT_WANDER_PROBE_DISTANCE = toFIXED(8),
  SCRIPT_WANDER_DIAGONAL_ANGLE = DEGtoANG(45), ///< one eighth of a turn
  SCRIPT_WANDER_SIDE_ANGLE = DEGtoANG(90),     ///< one quarter of a turn
  SCRIPT_HALF_TURN = DEGtoANG(180),
  /// Whole pixels SC_APPROACH_WATCHED stays short of its watched actor.
  SCRIPT_APPROACH_DISTANCE = 16,
  /// Under 64 pixels (16.16) SC_CAMERA measures its distance in 16.16.
  CAMERA_FOLLOW_FINE_DISTANCE = toFIXED(64),
  /// Within a quarter pixel (16.16) SC_CAMERA lands on its point.
  CAMERA_FOLLOW_SNAP_DISTANCE = toFIXED(1) / 4,
  /// Within half a pixel (16.16) SC_CAMERA takes the leader's height at once.
  CAMERA_FOLLOW_HEIGHT_SNAP = toFIXED(1) / 2
};

/// SC_LABEL and SC_TO differ only in bit 30; a jump searches for the label.
#define SCRIPT_LABEL_KEY_MASK 0xbfffffff

/// SC_WAIT(n): waits n frames; this frame counts as the first.
s32 ActorScript_Wait(struct Actor *interpreter) {
  s16 commandCursor = interpreter->scriptCursor;

  interpreter->wait = interpreter->script[commandCursor + 1] - 1;
  interpreter->scriptCursor += 2;
  return 0;
}

/// SC_SYNC: waits until the actor arrives at its target, or until a solid actor
/// has blocked it for 60 frames (refused is the blocked count, which
/// Actor_UpdateAllThumb raises).
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

/// SC_CALL(fn): calls fn with the actor.
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
s32 ActorScript_FindLabel(struct Actor *interpreter, u32 labelKey) {
  u32 *scriptWords;
  s32 wordIndex;

  interpreter->wait = 0;
  if (labelKey == 0) {
    return 0;
  }

  labelKey &= SCRIPT_LABEL_KEY_MASK;
  scriptWords = (u32 *)interpreter->script;
  for (wordIndex = 0; wordIndex < SCRIPT_LABEL_SEARCH_WORDS; wordIndex++) {
    if (*scriptWords++ == labelKey) {
      return wordIndex + 1;
    }
  }
  return 0;
}

/// SC_LOOP(n, to): jumps to label to until it has done so n - 1 times, then
/// moves on; with n = SC_FOREVER it always jumps.
s32 ActorScript_RepeatOrJump(struct Actor *interpreter) {
  const s32 *arguments;
  s32 repeatLimit;
  s32 jumpKey;
  s32 nextCount;

  arguments = &interpreter->script[interpreter->scriptCursor + 1];
  repeatLimit = *arguments++;
  jumpKey = *arguments;
  if (repeatLimit == SC_FOREVER) {
    interpreter->scriptCursor = ActorScript_FindLabel(interpreter, jumpKey);
  } else {
    nextCount = interpreter->repeatCount + 1;
    interpreter->repeatCount = nextCount;
    if ((u8)nextCount < (s16)repeatLimit) {
      interpreter->scriptCursor = ActorScript_FindLabel(interpreter, jumpKey);
    } else {
      interpreter->repeatCount = 0;
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
/// whose lone SC_END runs again each frame. Returns 0.
s32 ActorScript_End(struct Actor *interpreter) {
  interpreter->script = gActorScript_EndScript;
  interpreter->scriptCursor = 0;
  return 0;
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

/// SC_FLAG_ON(f): tests game flag f, then sets it.
SCRIPT_DEFINE_FLAG_COMMAND(ActorScript_SetFlagAndTest, GameFlag_SetFar)
/// SC_FLAG_OFF(f): tests game flag f, then clears it.
SCRIPT_DEFINE_FLAG_COMMAND(ActorScript_ClearFlagAndTest, GameFlag_ClearFar)

/// SC_FLAG_REV(f): tests game flag f, then flips it.
s32 ActorScript_ToggleFlagAndTest(struct Actor *interpreter) {
  s32 flagId;
  s32 previousFlagValue;

  flagId = interpreter->script[interpreter->scriptCursor + 1];
  previousFlagValue = GameFlag_GetFar(flagId);
  interpreter->conditionResult = previousFlagValue;
  if ((u8)previousFlagValue == 1) {
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

/// SC_RELEASE: releases the actor through Actor_Release, which releases its
/// sprites and zeroes its slot.
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
  Sound_PlayFar(interpreter->script[interpreter->scriptCursor + 1]);
  interpreter->scriptCursor += 2;
  return 1;
}

/// Returns -1 when an actor at position values would overlap a solid actor
/// (ACTOR_SOLID), each radius less 2 pixels; otherwise 0.
s32 ActorScript_CheckOverlap(struct Actor *actor, s32 *values) {
  s32 actorIndex;
  struct Actor *candidate = gActors;

  for (actorIndex = 0; actorIndex < ACTOR_COUNT; actorIndex++, candidate++) {
    if (candidate->script != NULL &&
        (candidate->collisionFlags & ACTOR_SOLID) && candidate != actor) {
      if (Field_CheckSpheresOverlap(&candidate->x.fixed, candidate->radius - 2,
                                    values, actor->radius - 2) >= 0)
        return -1;
    }
  }
  return 0;
}

/// Returns the first solid actor that an actor at position values would
/// overlap, as ActorScript_CheckOverlap tests, or NULL.
struct Actor *ActorScript_FindOverlappingEntry(struct Actor *actor,
                                               s32 *values) {
  s32 actorIndex;
  struct Actor *candidate = gActors;

  for (actorIndex = 0; actorIndex < ACTOR_COUNT; actorIndex++, candidate++) {
    if (candidate->script != NULL &&
        (candidate->collisionFlags & ACTOR_SOLID) && candidate != actor) {
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
s32 ActorScript_ApplyRelativePosition(struct Actor *actor) {
  u8 *commandBytes = (u8 *)(actor->script + actor->scriptCursor);
  s32 *arguments = (s32 *)(commandBytes + 4);
  s32 deltaX = *arguments++;
  s32 deltaY = *arguments++;
  s32 deltaZ = *arguments;

  Actor_SetMoveTarget(actor, actor->x.fixed + deltaX, actor->y.fixed + deltaY,
                      actor->z.fixed + deltaZ);
  actor->scriptCursor += 4;
  return 1;
}

/// SC_FACE_WATCHED: turns the actor to face its watched actor across the
/// ground.
s32 ActorScript_StoreAngleToWatchedActor(struct Actor *actor) {
  struct Actor *target;

  target = actor->watchedActor;
  actor->facing = Trig_Atan((target->z.fixed - actor->z.fixed),
                            (target->x.fixed - actor->x.fixed));
  actor->scriptCursor++;
  return 1;
}

/// SC_MOVE_TO_WATCHED: sends the actor towards its watched actor.
s32 ActorScript_ApplyWatchedActorPosition(struct Actor *actor) {
  struct Actor *target;

  target = actor->watchedActor;
  Actor_SetMoveTarget(actor, target->x.fixed, target->y.fixed, target->z.fixed);
  actor->scriptCursor++;
  return 1;
}

/// SC_MOVE_LOCAL(a, b): sends the actor towards its position plus an offset
/// that is never filled in; the operands are skipped unread.
s32 ActorScript_ApplyLocalOffsetPosition(struct Actor *actor) {
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

/// SC_CAMERA: keeps the camera actor on its leader (watchedActor), clamped so
/// the view stays on the map.
s32 FieldCamera_FollowLeaderInBounds(struct Actor *cameraActor) {
  struct Actor *leader;
  struct MapGridScrollState *bounds;
  s32 minX;
  FIXED minZ;
  s32 maxX;
  FIXED maxZ;
  FIXED x;
  FIXED y;
  FIXED z;
  s32 dz;
  s32 dy;
  s32 dx;
  FIXED distance;
  FIXED moveStep;

  // Use MapGrid_UpdateLayerScroll's viewport origin and the leader's elevation
  // when clamping its ground position to the viewport bounds.
  bounds = gMapGridState[0];
  leader = cameraActor->watchedActor;
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
    cameraActor->targetX = ACTOR_NO_TARGET;
    cameraActor->targetY = ACTOR_NO_TARGET;
    cameraActor->targetZ = ACTOR_NO_TARGET;
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
      dx = (x - cameraActor->x.fixed) / FIXED_ONE;
      /* LEFTOVER(template): this whole-pixel dy, overwritten below and not used
       * for the distance. */
      dy = (y - cameraActor->y.fixed) / FIXED_ONE;
      dz = (z - cameraActor->z.fixed) / FIXED_ONE;
      distance = toFIXED(Iwram_Sqrt(dx * dx + dz * dz));
      dx = x - cameraActor->x.fixed;
      dy = y - cameraActor->y.fixed;
      dz = z - cameraActor->z.fixed;
      // The whole-pixel root is coarse; under 64 pixels the distance is
      // measured again in 16.16 for a finer result.
      if (distance < CAMERA_FOLLOW_FINE_DISTANCE)
        distance = Math_SqrtFX(Iwram_MulFX(dx, dx) + Iwram_MulFX(dz, dz));
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
          dx = Iwram_MulFX(Iwram_RatioMulQ14(distance, dx), moveStep);
          dz = Iwram_MulFX(Iwram_RatioMulQ14(distance, dz), moveStep);
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

/// SC_APPROACH_WATCHED: follows the watched actor at its speed.
s32 Actor_ApproachWatchedActor(struct Actor *actor) {
  s32 distancePixels;
  FIXED moveZ;
  FIXED dx;
  FIXED dz;
  s32 deltaZPixels;
  s32 deltaXPixels;
  s32 remainingDistance;
  FIXED moveX;
  struct Actor *watchedActor;

  watchedActor = actor->watchedActor;
  actor->speedLimit = watchedActor->speedLimit;
  actor->acceleration = watchedActor->acceleration;
  dx = watchedActor->x.fixed - actor->x.fixed;
  dz = watchedActor->z.fixed - actor->z.fixed;
  deltaXPixels = FIXEDtoINT(dx);
  deltaZPixels = FIXEDtoINT(dz);
  distancePixels =
      Iwram_Sqrt((deltaXPixels * deltaXPixels) + (deltaZPixels * deltaZPixels));
  // Further than 16 pixels: aim at the point 16 pixels short of the watched
  // actor along the line to it (the 16.16 offset scaled by the whole-pixel
  // ratio) and run on to SC_WAIT(1); otherwise stand and stay on this command.
  if (distancePixels > SCRIPT_APPROACH_DISTANCE) {
    remainingDistance = distancePixels - SCRIPT_APPROACH_DISTANCE;
    moveX = dx * remainingDistance / distancePixels;
    moveZ = dz * remainingDistance / distancePixels;
    Actor_SetMoveTarget(actor, actor->x.fixed + moveX, actor->y.fixed,
                        actor->z.fixed + moveZ);
    Actor_SelectAnimation(actor, ANIM_WALK);
    actor->scriptCursor++;
    return 1;
  }
  Actor_SelectAnimation(actor, ANIM_STAND);
  return 0;
}

/// SC_WANDER(base, range, limit): walks to a random point base to base + range
/// away, within 90 degrees either side of the way it faces, that no
/// solid actor or wall blocks, with clear ground 8 pixels further at 45 degrees
/// either side, and within limit pixels of home.
s32 Actor_Wander(struct Actor *actor) {
  struct FieldPosition candidatePosition;
  struct FieldPosition probePosition;
  s32 minimumDistance;
  const s32 *operands;
  s32 distanceRange;
  s32 homeRadiusSquared;
  FIXED travelDistance;
  s32 heading;
  s32 attempts;
  s32 dx;
  s32 dz;
#ifdef BUGFIX
  s32 aheadBlocked;
#endif

  // base and range are 16.16 pixels; limit is compared in whole pixels,
  // against homeX and homeZ.
  operands = &actor->script[actor->scriptCursor + 1];
  minimumDistance = *operands++;
  distanceRange = *operands++;
  homeRadiusSquared = *operands / FIXED_ONE;
  attempts = 0;
  homeRadiusSquared = homeRadiusSquared * homeRadiusSquared;
retry:
  attempts++;
  if (attempts <= SCRIPT_WANDER_TRIES) {
    SET_POSITION(&candidatePosition, actor->x.fixed, actor->y.fixed,
                 actor->z.fixed);
    // Random16 is a 16.16 fraction below 1, so the distance is base plus up to
    // range.
    travelDistance = minimumDistance + Iwram_MulFX(Random16(), distanceRange);
    heading = actor->facing + (Random16() >> 2) - (Random16() >> 2);
    Vector_AddPolarOffset(travelDistance, heading, &candidatePosition);
    if (ActorScript_CheckOverlap(actor, (s32 *)&candidatePosition) != 0 ||
        MapGrid_CheckCollision(actor, &candidatePosition) != COLLISION_NONE)
      goto retry;
    // Probes 8 pixels past the point, ahead and at 45 degrees (DEGtoANG(45))
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
    /* FAKEMATCH(dead): the offsets in both arms of a test of actor. */
    if (actor != 0) {
      dx = candidatePosition.x / FIXED_ONE - actor->homeX;
      dz = candidatePosition.z / FIXED_ONE - actor->homeZ;
    } else {
      dx = candidatePosition.x / FIXED_ONE - actor->homeX;
      dz = candidatePosition.z / FIXED_ONE - actor->homeZ;
    }
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

/// SC_WANDER_HOME(base, range, limit): wanders as SC_WANDER does, testing more
/// ground around the point, while it is within limit pixels of home; outside
/// that, or when no point works, it heads back towards home.
s32 ActorScript_WanderNearHome(struct Actor *actor) {
  struct FieldPosition candidatePosition;
  struct FieldPosition probePosition;
  s32 minimumDistance;
  const s32 *operands;
  s32 distanceRange;
  s32 homeRadiusSquared;
  FIXED travelDistance;
  u16 heading;
  u16 angle;
  s32 attempts;
  s32 dx;
  s32 dz;

  operands = &actor->script[actor->scriptCursor + 1];
  minimumDistance = *operands++;
  distanceRange = *operands++;
  homeRadiusSquared = *operands / FIXED_ONE;
  angle = actor->facing;
  homeRadiusSquared *= homeRadiusSquared;
  attempts = 0;
  dx = actor->x.fixed / FIXED_ONE - actor->homeX;
  dz = actor->z.fixed / FIXED_ONE - actor->homeZ;
  if (dx * dx + dz * dz <= homeRadiusSquared) {
  roam:
    attempts++;
    if (attempts <= SCRIPT_WANDER_TRIES) {
      travelDistance = minimumDistance + Iwram_MulFX(Random16(), distanceRange);
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
      dx = candidatePosition.x / FIXED_ONE - actor->homeX;
      dz = candidatePosition.z / FIXED_ONE - actor->homeZ;
      if (dx * dx + dz * dz > homeRadiusSquared)
        goto roam;
      actor->collisionFlags |= ACTOR_PUSHABLE;
      Actor_SetMoveTarget(actor, candidatePosition.x, candidatePosition.y,
                          candidatePosition.z);
      goto done;
    }
  }
  // Head back: aim opposite the angle from home to the actor (dx and dz as
  // measured at the start, or at the last point found too far from home), with
  // the same spread, testing only the near and far points.
  attempts = 0;
  angle = (u16)Trig_Atan(dz, dx) + SCRIPT_HALF_TURN;
back:
  attempts++;
  if (attempts <= SCRIPT_WANDER_TRIES) {
    travelDistance = minimumDistance + Iwram_MulFX(Random16(), distanceRange);
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
    actor->collisionFlags &= ~ACTOR_PUSHABLE;
    Actor_SetMoveTarget(actor, candidatePosition.x, candidatePosition.y,
                        candidatePosition.z);
  }
done:
  actor->scriptCursor += 4;
  return 1;
}

/// What a state value handler does with its value, for SC_SET, SC_ADD and
/// SC_CMP in turn.
enum ScriptOperandOperation {
  SCRIPT_OPERAND_SET,
  SCRIPT_OPERAND_ADD,
  SCRIPT_OPERAND_COMPARE ///< sets conditionResult to 1 when equal, else 0
};

/// Defines the handler for a 32-bit state value: set it, add to it, or compare
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

/// The state value handlers, one for each entry of enum ActorScriptValue (see
/// gActorScript_OperandHandlerTable in actor_script_operands.c); each names the
/// actor field it sets, adds to or compares.
SCRIPT_DEFINE_OPERAND_ADDRESS(ActorScript_SetOrCompareScriptAddress,
                              scriptAddress)

/// State value SW_CURSOR: sets, adds to or compares the script cursor.
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

/// State value SW_FACING (and 2 and 4): sets, adds to or compares the actor's
/// facing.
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareFacing, facing, u16)
/// State value SW_X: sets, adds to or compares the actor's x.
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareX, x)
/// State value SW_Y: sets, adds to or compares the actor's y.
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareY, y)
/// State value SW_Z: sets, adds to or compares the actor's z.
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareZ, z)

/// State value SW_RADIUS: sets, adds to or compares the actor's radius.
void ActorScript_SetOrCompareRadius(struct ScriptOperands *state, s32 operation,
                                    s32 value) {
  if (operation == SCRIPT_OPERAND_SET) {
    state->radius = value;
    return;
  }
  if (operation == SCRIPT_OPERAND_ADD) {
    state->radius += value;
    return;
  }
  state->conditionResult = state->radius == (s16)value;
}

/// State value SW_SCALE_X: sets, adds to or compares the actor's horizontal
/// scale.
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareScaleX, scaleX)
/// State value SW_SCALE_Y (and 11): sets, adds to or compares the actor's
/// vertical scale.
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareScaleY, scaleY)
/// State value SW_VELOCITY_X: sets, adds to or compares the actor's x velocity.
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareVelocityX, velocityX)
/// State value SW_VELOCITY_Y: sets, adds to or compares the actor's y velocity.
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareVelocityY, velocityY)
/// State value SW_VELOCITY_Z: sets, adds to or compares the actor's z velocity.
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareVelocityZ, velocityZ)
/// State value SW_SPEED: sets, adds to or compares the actor's top speed.
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareSpeedLimit, speedLimit)
/// State value SW_ACCELERATION: sets, adds to or compares the actor's
/// acceleration.
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareAcceleration, acceleration)
/// State value SW_TARGET_X: sets, adds to or compares the x the actor heads
/// for.
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareTargetX, targetX)
/// State value SW_TARGET_Y: sets, adds to or compares the y the actor heads
/// for.
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareTargetY, targetY)
/// State value SW_TARGET_Z: sets, adds to or compares the z the actor heads
/// for.
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareTargetZ, targetZ)
/// State value SW_BOUNCE: sets, adds to or compares the actor's bounce.
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareVerticalMotion, bounce)
/// State value SW_GRAVITY: sets, adds to or compares the actor's gravity.
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareGravity, gravity)
/// State value SW_GROUND: sets, adds to or compares the ground height under the
/// actor.
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareTerrainHeight, groundHeight)
/// State value 23: sets, adds to or compares the actor's sceneWord, scene
/// code's own word.
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareSceneWord, sceneWord)
/// State value 24: sets, adds to or compares the actor's sprite.
SCRIPT_DEFINE_OPERAND_ADDRESS(ActorScript_SetOrCompareSprite, sprite)
/// State value 25: sets, adds to or compares whether the actor draws no sprite,
/// one or a group.
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareSpriteStorageKind,
                             spriteStorageKind, u8)
/// State value SW_MOTION: sets, adds to or compares the actor's motion flags.
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareMotionFlags, motionFlags,
                             u8)
/// State value SW_ARRIVAL: sets, adds to or compares the axis the actor's
/// arrival is judged on.
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareArrivalAxis, arrivalAxis,
                             u8)
/// State value SW_RESULT: sets, adds to or compares the last comparison's
/// result.
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareComparisonResult,
                             conditionResult, u8)
/// State value SW_SNAP: sets, adds to or compares whether the actor snaps to
/// its target.
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareSnapToTarget, snapToTarget,
                             u8)
/// State value SW_COLLISION: sets, adds to or compares the actor's collision
/// flags.
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareCollisionFlags,
                             collisionFlags, u8)
/// State value SW_STEERING: sets, adds to or compares the actor's steering
/// flags.
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareSteeringFlags,
                             steeringFlags, u8)
/// State value SW_PAUSED: sets, adds to or compares whether the actor is
/// paused.
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrComparePaused, paused, u8)
/// State value SW_REPEAT: sets, adds to or compares the script's repeat count.
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareRepeatCount, repeatCount,
                             u8)

/// State value SW_WAIT: sets, adds to or compares the wait counter.
void ActorScript_SetOrCompareWait(struct ScriptOperands *state, s32 operation,
                                  s32 value) {
  if (operation == SCRIPT_OPERAND_SET) {
    state->wait = value;
    return;
  }
  if (operation == SCRIPT_OPERAND_ADD) {
    state->wait += value;
    return;
  }
  state->conditionResult = (s16)state->wait == (s16)value;
}

/// State value SW_TIMER: sets, adds to or compares the actor's timer.
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareTimer, timer, s16)
/// State value SW_USER_VALUE: sets, adds to or compares the actor's user value.
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareUserValue, userValue, s16)
/// State value 37: sets, adds to or compares the actor this one watches.
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareWatchedActor, watchedActor)
/// State value SW_HOOK: sets, adds to or compares the actor's update routine.
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareUpdateAddress, update)
/// State value SW_SCENE_COUNTER: sets, adds to or compares the actor's rise
/// counter.
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareSceneCounter, sceneCounter,
                             u8)
/// State value SW_SCENE_BYTE: sets, adds to or compares whether the actor
/// rises.
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareSceneByte, sceneByte, u8)

/// Reads the two words after the command (a state value number and a value) and
/// passes operation to that value's handler.
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

/// SC_SET(w, v): sets state value w to v.
s32 ActorScript_ApplyOperandSet(struct Actor *interpreter) {
  struct ScriptOperands *operands = (struct ScriptOperands *)interpreter;

  return ActorScript_ApplyOperandCommand(operands, SCRIPT_OPERAND_SET);
}

/// SC_ADD(w, v): adds v to state value w.
s32 ActorScript_ApplyOperandAdd(struct Actor *interpreter) {
  struct ScriptOperands *operands = (struct ScriptOperands *)interpreter;

  return ActorScript_ApplyOperandCommand(operands, SCRIPT_OPERAND_ADD);
}

/// SC_CMP(w, v): tests whether state value w equals v.
s32 ActorScript_ApplyOperandCompare(struct Actor *interpreter) {
  struct ScriptOperands *operands = (struct ScriptOperands *)interpreter;

  return ActorScript_ApplyOperandCommand(operands, SCRIPT_OPERAND_COMPARE);
}

#undef SCRIPT_DEFINE_FLAG_COMMAND
#undef SCRIPT_DEFINE_OPERAND_WORD
#undef SCRIPT_DEFINE_OPERAND_ADDRESS
#undef SCRIPT_DEFINE_OPERAND_NARROW
#undef SCRIPT_LABEL_KEY_MASK
