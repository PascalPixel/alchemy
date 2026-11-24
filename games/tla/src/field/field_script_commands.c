/// @file
/// Actor script commands: waits, jumps, flags, moves and work values.
///
/// Each `SC_` handler here is reached through gActorScript_CommandTable
/// (map_grid_data.c), which Actor_UpdateAllThumb calls for each live actor;
/// each returns 1 to run on and 0 to end the actor's frame (the protocol is
/// in this folder's README).
// Part of field.c.

#include "field_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "field/flag_call.h"
#include "field/field.h"
#include "field/resource_slot.h"
#include "system/system.h"
#include "system/scene.h"
#include "macros.h"
#include "field/field_event.h"
#include "system/input.h"
#include "system/frame.h"
#include "field/field_sequence_event.h"
#include "game/class_djinn.h"

/// The actor script commands' limits, angles and distances.
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

/// Advances the motion of those of the first RUNNING_LIST_SLOTS actors whose
/// script is running by one frame, without running their scripts: steering
/// towards a target or braking, gravity and bouncing, arrival, and turning to
/// face the direction of travel.
void Actor_UpdateAllMotion(void) {
  struct Actor *list[RUNNING_LIST_SLOTS + 1];
  struct Actor **next;
  struct Actor *actor;
  FIXED y;
  s32 arrivedAtTarget;
  FIXED x;
  FIXED z;
  s32 dx;
  s32 dz;
  s32 distance;
  FIXED ratio;
  s32 facingStep;

  // Those of the first RUNNING_LIST_SLOTS actors whose script is running.
  actor = Ram_HeapSlots->actors;
  Actor_ListRunning(actor, list);
  actor = list[0];
  next = &list[1];
  while (actor != NULL) {
    x = actor->x.fixed;
    y = actor->y.fixed;
    z = actor->z.fixed;
    // UB: a frozen actor otherwise reads an uninitialized arrival flag, or
    // inherits the preceding actor's flag.
#ifdef UBFIX
    arrivedAtTarget = 0;
#endif
    if (actor->frozen == 0) {
#ifndef UBFIX
      arrivedAtTarget = 0;
#endif
      // Steer in the XZ plane towards the target, or brake.
      if (actor->targetX != ACTOR_NO_TARGET) {
        dx = (actor->targetX - x) / FIXED_ONE;
        dz = (actor->targetZ - z) / FIXED_ONE;
        distance = toFIXED(Iwram_Sqrt(dx * dx + dz * dz));
        // Under 256 pixels, measure again in 16.16: the root of a 16.16
        // square is 8.8, and << 8 makes it 16.16 again.
        if (distance <= toFIXED(256) - 1) {
          dx = actor->targetX - x;
          dz = actor->targetZ - z;
          distance = Iwram_Sqrt(Iwram_MulFX(dx, dx) + Iwram_MulFX(dz, dz)) << 8;
        }
        if (distance == 0) {
          x = actor->targetX;
          z = actor->targetZ;
        } else {
          ratio = Iwram_RatioMulQ14(distance, actor->acceleration);
          actor->velocityX += Iwram_MulFX(dx, ratio);
          actor->velocityZ += Iwram_MulFX(dz, ratio);
          dx = actor->velocityX;
          dz = actor->velocityZ;
          distance = Iwram_Sqrt(Iwram_MulFX(dx, dx) + Iwram_MulFX(dz, dz)) << 8;
          if (distance > actor->speedLimit) {
            ratio = Iwram_RatioMulQ14(distance, actor->speedLimit);
            actor->velocityX = Iwram_MulFX(dx, ratio);
            actor->velocityZ = Iwram_MulFX(dz, ratio);
          }
        }
      } else {
        dx = actor->velocityX;
        dz = actor->velocityZ;
        if ((dx | dz) != 0) {
          distance = Iwram_Sqrt(Iwram_MulFX(dx, dx) + Iwram_MulFX(dz, dz)) << 8;
          if (distance != 0) {
            if (distance - actor->acceleration < 0) {
              actor->velocityX = 0;
              actor->velocityZ = 0;
            } else {
              ratio =
                  Iwram_RatioMulQ14(distance, distance - actor->acceleration);
              actor->velocityX = Iwram_MulFX(dx, ratio);
              actor->velocityZ = Iwram_MulFX(dz, ratio);
            }
          } else {
            actor->velocityX = 0;
            actor->velocityZ = 0;
          }
        }
      }
      // Gravity, and a bounce that loses energy on landing.
      if (actor->motionFlags & ACTOR_FALLS) {
        if (y > actor->groundHeight) {
          actor->velocityY -= actor->gravity;
        } else if (actor->velocityY < 0) {
          y = actor->groundHeight;
          actor->velocityY = -Iwram_MulFX(actor->velocityY, actor->bounce);
          if ((actor->velocityY < 0 ? -actor->velocityY : actor->velocityY) <=
              actor->gravity)
            actor->velocityY = 0;
        }
      }
    }
    x += actor->velocityX;
    y += actor->velocityY;
    z += actor->velocityZ;
    // Arrived: the watched axis reached the target, or crossed it.
    if (actor->arrivalAxis != 0)
      switch (actor->arrivalAxis) {
      case ARRIVAL_AXIS_X:
        if (x == actor->targetX ||
            ((actor->x.fixed - actor->targetX) ^ (x - actor->targetX)) < 0)
          arrivedAtTarget = 1;
        break;
      case ARRIVAL_AXIS_Y:
        if (y == actor->targetY ||
            ((actor->y.fixed - actor->targetY) ^ (y - actor->targetY)) < 0)
          arrivedAtTarget = 1;
        break;
      case ARRIVAL_AXIS_Z:
        if (z == actor->targetZ ||
            ((actor->z.fixed - actor->targetZ) ^ (z - actor->targetZ)) < 0)
          arrivedAtTarget = 1;
        break;
      }
    if (arrivedAtTarget) {
      if (actor->snapToTarget) {
        actor->velocityX = 0;
        actor->velocityZ = 0;
        x = actor->targetX;
        z = actor->targetZ;
        if (actor->motionFlags == 0) {
          y = actor->targetY;
          actor->velocityY = 0;
        }
      }
      actor->targetX = ACTOR_NO_TARGET;
      actor->targetY = ACTOR_NO_TARGET;
      actor->targetZ = ACTOR_NO_TARGET;
      actor->arrivalAxis = 0;
    }
    SET_ACTOR_POSITION(actor, x, y, z);
    // Turn to face the direction of travel, at most a sixteenth of a turn a
    // frame, the shorter way round.
    if (actor->steeringFlags & ACTOR_FACES_MOTION) {
      x = actor->velocityX;
      z = actor->velocityZ;
      if (x != 0 || z != 0) {
        facingStep = (s16)(Trig_Atan(z, x) - actor->facing);
        if (facingStep > FACING_STEP)
          facingStep = FACING_STEP;
        if (facingStep < -FACING_STEP)
          facingStep = -FACING_STEP;
        actor->facing += facingStep;
      }
    }
    actor = *next++;
  }
}

/// SC_WAIT(n): waits n frames; this frame counts as the first.
s32 ActorScript_Wait(struct Actor *interpreter) {
  s16 commandCursor = interpreter->scriptCursor;

  interpreter->wait = interpreter->script[commandCursor + 1] - 1;
  interpreter->scriptCursor += 2;
  return 0;
}

/// SC_SYNC: waits until the actor arrives at its target, or until a solid actor
/// has blocked it for SCRIPT_BLOCKED_MOVE_FRAMES frames (refused is the
/// blocked count, which Actor_UpdateAllThumb raises), when its target is
/// cleared.
s32 ActorScript_WaitForArrival(struct Actor *interpreter) {
  struct WaitingActor *actor = (struct WaitingActor *)interpreter;

  if (actor->refused > SCRIPT_BLOCKED_MOVE_FRAMES - 1) {
    actor->refused = 0;
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
    actor->arrivalAxis = 0;
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
/// matched without the bit that tells them apart), searching its first
/// SCRIPT_LABEL_SEARCH_WORDS words, or 0 (the start) when key is 0 or not
/// found, so a missing label restarts the script. Clears the wait.
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
    if ((s32)(u8)nextCount < (s32)(s16)repeatLimit) {
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

/// SC_END: hands the actor back to the idle script, gActorScript_IdleScript,
/// whose lone SC_END runs again each frame. Returns 0.
s32 ActorScript_End(struct Actor *interpreter) {
  s32 result;

  interpreter->script = gActorScript_IdleScript;
  interpreter->scriptCursor = (result = 0);
  return result;
}

/// SC_FLAG_CHECK(f): tests game flag f.
s32 ActorScript_TestFlag(struct Actor *interpreter) {
  s32 done;

  interpreter->conditionResult =
      GameFlag_Get(interpreter->script[interpreter->scriptCursor + 1]);
  done = 1;
  interpreter->scriptCursor = (u16)interpreter->scriptCursor + 2;
  return done;
}

/// SC_FLAG_ON(f): tests game flag f, then sets it.
s32 ActorScript_SetFlagAndTest(struct Actor *interpreter) {
  s32 value;

  value = interpreter->script[interpreter->scriptCursor + 1];
  interpreter->conditionResult = GameFlag_Get(value);
  GameFlag_Set(value);
  interpreter->scriptCursor += 2;
  return 1;
}

/// SC_FLAG_OFF(f): tests game flag f, then clears it.
s32 ActorScript_ClearFlagAndTest(struct Actor *interpreter) {
  s32 value;

  value = interpreter->script[interpreter->scriptCursor + 1];
  interpreter->conditionResult = GameFlag_Get(value);
  GameFlag_Clear(value);
  interpreter->scriptCursor += 2;
  return 1;
}

/// SC_FLAG_REV(f): tests game flag f, then flips it.
s32 ActorScript_ToggleFlagAndTest(struct Actor *interpreter) {
  s32 value;

  value = interpreter->script[interpreter->scriptCursor + 1];
  interpreter->conditionResult = GameFlag_Get(value);
  if (interpreter->conditionResult == 1)
    GameFlag_Clear(value);
  else
    GameFlag_Set(value);
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

/// SC_SE_IN_VIEW(n): plays sound n when the actor is in view of the camera
/// (Actor_PlaySoundInView).
s32 ActorScript_PlaySoundInView(struct Actor *interpreter) {
  Actor_PlaySoundInViewFar(interpreter,
                           interpreter->script[interpreter->scriptCursor + 1]);
  interpreter->scriptCursor += 2;
  return 1;
}

/// Returns -1 when an actor at position values would overlap a solid actor,
/// each radius less 2 pixels; otherwise 0.
s32 ActorScript_CheckOverlap(struct Actor *actor, s32 *values) {
  s32 actorIndex;
  struct Actor *candidate = ((struct Actor *)Ram_HeapSlots->actors);

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
struct Actor *ActorScript_FindOverlappingActor(struct Actor *actor,
                                               s32 *values) {
  s32 actorIndex;
  struct Actor *candidate = ((struct Actor *)Ram_HeapSlots->actors);

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

/// SC_POSITION(x, y, z): places the actor at (x, y, z) and stops it; with y -1
/// an actor that follows the ground is raised by the ground height there.
s32 ActorScript_SetPositionAndResetMotion(struct Actor *actor) {
  const s32 *argument = actor->script + (s16)actor->scriptCursor + 1;
  s32 x = *argument++;
  s32 y = *argument++;
  s32 z = *argument;
  FIXED height;

  Actor_SetPositionAndResetMotion(actor, x, y, z);
  if (y == -1 && (actor->motionFlags & ACTOR_FOLLOWS_TERRAIN)) {
    height =
        MapGrid_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
    actor->y.fixed = actor->y.fixed - actor->groundHeight + height;
    actor->groundHeight = height;
  }
  actor->scriptCursor += 4;
  return 1;
}

/// SC_POSITION_ORIGIN: puts the actor at x and z 0, keeping its height, with
/// no destination or speed; one that follows the ground then takes the ground
/// height there.
s32 ActorScript_ResetToOrigin(struct Actor *actor) {
  FIXED height;

  Actor_SetPositionAndResetMotion(actor, 0, actor->y.fixed, 0);
  if (actor->motionFlags & ACTOR_FOLLOWS_TERRAIN) {
    height =
        MapGrid_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
    actor->y.fixed = actor->y.fixed - actor->groundHeight + height;
    actor->groundHeight = height;
  }
  actor->scriptCursor += 1;
  return 1;
}

/// SC_MOVE(x, y, z): sends the actor towards (x, y, z).
s32 ActorScript_ApplyAbsolutePosition(struct Actor *actor) {
  const s32 *argument = actor->script + (s16)actor->scriptCursor + 1;
  s32 x = *argument++;
  s32 y = *argument++;
  s32 z = *argument;

  Actor_SetMoveTarget(actor, x, y, z);
  actor->scriptCursor += 4;
  return 1;
}

/// SC_MOVE_XZ(x, z): sends the actor towards (x, z), keeping its height.
s32 ActorScript_SetMoveTargetXZ(struct Actor *actor) {
  const s32 *argument = actor->script + (s16)actor->scriptCursor + 1;
  s32 x = *argument++;
  s32 z = *argument;

  Actor_SetMoveTarget(actor, x, actor->y.fixed, z);
  actor->scriptCursor += 3;
  return 1;
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

/// SC_FACE_WATCHED: turns the actor to face its watched actor, on the ground
/// plane (x and z).
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

/// How far apart a and b are.
#define DISTANCE(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))

/// SC_CAMERA: keeps the camera actor on its leader (watchedActor), clamped so
/// the view stays on the map, and pushed out of the map's camera boxes, which
/// the view may not show.
s32 FieldCamera_FollowLeaderInBounds(struct Actor *interpreter) {
  struct Actor *camera = interpreter;
  struct Actor *leader;
  struct MapState *bounds;
  FIXED minX;
  FIXED minZ;
  FIXED maxX;
  FIXED maxZ;
  FIXED x;
  FIXED y;
  FIXED z;
  s32 dz;
  s32 dy;
  s32 dx;
  FIXED distance;
  FIXED moveStep;
  s16 *region;

  bounds = Ram_HeapSlots->mapGrid;
  leader = camera->watchedActor;
  // UB: the bounds read the leader's height before the null check below.
#ifdef UBFIX
  if (leader == NULL) {
    camera->motionFlags = 0;
    camera->scriptCursor++;
    return 1;
  }
#endif
  // A layered map draws a point at screen row z - y, so the z bounds move
  // with the leader's height and the clamp keeps that row inside the map.
  minX = bounds->minX + FIELD_VIEW_FOCUS_X;
  minZ = bounds->minZ + leader->y.fixed + FIELD_VIEW_FOCUS_Y;
  maxX = bounds->maxX - FIELD_VIEW_FOCUS_X;
  maxZ =
      bounds->maxZ + leader->y.fixed - (FIELD_VIEW_HEIGHT - FIELD_VIEW_FOCUS_Y);
  camera->motionFlags = 0;
  if (leader != NULL && leader->script != NULL) {
    x = leader->x.fixed;
    y = leader->y.fixed;
    z = leader->z.fixed;
    camera->targetX = ACTOR_NO_TARGET;
    camera->targetY = ACTOR_NO_TARGET;
    camera->targetZ = ACTOR_NO_TARGET;
    if (x < minX)
      x = minX;
    if (z < minZ)
      z = minZ;
    if (x > maxX)
      x = maxX;
    if (z > maxZ)
      z = maxZ;
    if (bounds->cameraBoxes != NULL) {
      region = bounds->cameraBoxes;
      while (region[0] != -1) {
        // The points from which the view would show some of the box.
        minX = toFIXED(region[0]) - FIELD_VIEW_FOCUS_X;
        minZ = toFIXED(region[1]) + leader->y.fixed -
               (FIELD_VIEW_HEIGHT - FIELD_VIEW_FOCUS_Y);
        maxX = toFIXED(region[2]) + FIELD_VIEW_FOCUS_X;
        maxZ = toFIXED(region[3]) + leader->y.fixed + FIELD_VIEW_FOCUS_Y;
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
    if (camera->timer != 0) {
      SET_ACTOR_POSITION(camera, x, y, z);
    } else {
      dx = (x - camera->x.fixed) / FIXED_ONE;
      /* LEFTOVER(template): this whole-pixel dy, overwritten below and not used
       * for the distance; Camelot's template: the sibling routine
       * Actor_SetMoveTarget (field_actor_motion.c) computes the same three
       * whole-pixel deltas and uses y's in its distance; as the line removed.
       */
      dy = (y - camera->y.fixed) / FIXED_ONE;
      dz = (z - camera->z.fixed) / FIXED_ONE;
      distance = toFIXED(Iwram_Sqrt(dx * dx + dz * dz));
      dx = x - camera->x.fixed;
      dy = y - camera->y.fixed;
      dz = z - camera->z.fixed;
      if (distance < CAMERA_FOLLOW_FINE_DISTANCE)
        distance = Math_SqrtFX(IwramMulFX(dx, dx) + IwramMulFX(dz, dz));
      if ((moveStep = distance / 8) > camera->speedLimit)
        moveStep = camera->speedLimit;
      if (distance < CAMERA_FOLLOW_SNAP_DISTANCE) {
        camera->x.fixed = x;
        camera->z.fixed = z;
      } else {
        if (distance > moveStep) {
          FIXED ratio = Iwram_RatioMulQ14(distance, dx);
          s32 (*mulFX)(s32, s32) = IwramMulFX;
          dx = mulFX(ratio, moveStep);
          dz = mulFX(Iwram_RatioMulQ14(distance, dz), moveStep);
        }
        camera->x.fixed += dx;
        camera->z.fixed += dz;
      }
      if ((dy < 0 ? -dy : dy) > CAMERA_FOLLOW_HEIGHT_SNAP)
        dy /= 4;
      camera->y.fixed += dy;
    }
  }
  camera->scriptCursor++;
  return 1;
}

/// SC_CAMERA_IN_BOUNDS: as SC_CAMERA, clamped to the map alone, without its
/// camera boxes.
s32 FieldCamera_FollowWatchedInBounds(struct Actor *camera) {
  struct Actor *leader;
  struct MapState *bounds;
  FIXED minX;
  FIXED minZ;
  FIXED maxX;
  FIXED maxZ;
  FIXED x;
  FIXED y;
  FIXED z;
  s32 dz;
  s32 dy;
  s32 dx;
  FIXED distance;
  FIXED moveStep;

  bounds = Ram_HeapSlots->mapGrid;
  leader = camera->watchedActor;
  minX = bounds->minX + FIELD_VIEW_FOCUS_X;
  minZ = bounds->minZ + FIELD_VIEW_FOCUS_Y;
  maxX = bounds->maxX - FIELD_VIEW_FOCUS_X;
  maxZ = bounds->maxZ - (FIELD_VIEW_HEIGHT - FIELD_VIEW_FOCUS_Y);
  camera->motionFlags = 0;
  if (leader != NULL && leader->script != NULL) {
    x = leader->x.fixed;
    y = leader->y.fixed;
    z = leader->z.fixed;
    camera->targetX = ACTOR_NO_TARGET;
    camera->targetY = ACTOR_NO_TARGET;
    camera->targetZ = ACTOR_NO_TARGET;
    if (x < minX)
      x = minX;
    if (z < minZ)
      z = minZ;
    if (x > maxX)
      x = maxX;
    if (z > maxZ)
      z = maxZ;
    if (camera->timer != 0) {
      SET_ACTOR_POSITION(camera, x, y, z);
    } else {
      dx = (x - camera->x.fixed) / FIXED_ONE;
      /* LEFTOVER(template): this whole-pixel dy, overwritten below and not used
       * for the distance; Camelot's template: the sibling routine
       * Actor_SetMoveTarget (field_actor_motion.c) computes the same three
       * whole-pixel deltas and uses y's in its distance; as the line removed.
       */
      dy = (y - camera->y.fixed) / FIXED_ONE;
      dz = (z - camera->z.fixed) / FIXED_ONE;
      distance = toFIXED(Iwram_Sqrt(dx * dx + dz * dz));
      dx = x - camera->x.fixed;
      dy = y - camera->y.fixed;
      dz = z - camera->z.fixed;
      if (distance < CAMERA_FOLLOW_FINE_DISTANCE)
        distance = Math_SqrtFX(Iwram_MulFX(dx, dx) + Iwram_MulFX(dz, dz));
      if ((moveStep = distance / 8) > camera->speedLimit)
        moveStep = camera->speedLimit;
      if (distance < CAMERA_FOLLOW_SNAP_DISTANCE) {
        camera->x.fixed = x;
        camera->z.fixed = z;
      } else {
        if (distance > moveStep) {
          dx = Iwram_MulFX(Iwram_RatioMulQ14(distance, dx), moveStep);
          dz = Iwram_MulFX(Iwram_RatioMulQ14(distance, dz), moveStep);
        }
        camera->x.fixed += dx;
        camera->z.fixed += dz;
      }
      if ((dy < 0 ? -dy : dy) > CAMERA_FOLLOW_HEIGHT_SNAP)
        dy /= 4;
      camera->y.fixed += dy;
    }
  }
  camera->scriptCursor++;
  return 1;
}

/// SC_APPROACH_WATCHED: follows the watched actor at its speed. Further than
/// SCRIPT_APPROACH_DISTANCE away it walks to that distance short of it and runs
/// on; nearer, it stands and waits for the next frame.
s32 Actor_ApproachWatchedActor(struct Actor *actor) {
  s32 distancePixels;
  FIXED moveZ;
  FIXED dx;
  FIXED dz;
  s32 deltaZPixels;
  s32 deltaXPixels;
  s32 remainingDistance;
  FIXED moveX;
  struct Actor *watchedObject;

  watchedObject = actor->watchedActor;
  actor->speedLimit = watchedObject->speedLimit;
  actor->acceleration = watchedObject->acceleration;
  dx = watchedObject->x.fixed - actor->x.fixed;
  dz = watchedObject->z.fixed - actor->z.fixed;
  deltaXPixels = FIXEDtoINT(dx);
  deltaZPixels = FIXEDtoINT(dz);
  distancePixels =
      Iwram_Sqrt((deltaXPixels * deltaXPixels) + (deltaZPixels * deltaZPixels));
  if (distancePixels > SCRIPT_APPROACH_DISTANCE) {
    remainingDistance = distancePixels - SCRIPT_APPROACH_DISTANCE;
    moveX = dx * remainingDistance / distancePixels;
    moveZ = dz * remainingDistance / distancePixels;
    Actor_SetMoveTarget(actor, actor->x.fixed + moveX, actor->y.fixed,
                        actor->z.fixed + moveZ);
    Actor_SelectAnimation(actor, ANIM_WALK);
    actor->scriptCursor = (actor->scriptCursor + 1);
    return 1;
  }
  Actor_SelectAnimation(actor, ANIM_STAND);
  return 0;
}

/// SC_WANDER(base, range, limit): walks to a random point base to base +
/// range away, within SCRIPT_WANDER_SIDE_ANGLE either side of its facing, that
/// no solid actor or wall blocks, with clear ground
/// SCRIPT_WANDER_PROBE_DISTANCE further at SCRIPT_WANDER_DIAGONAL_ANGLE either
/// side, and within limit pixels of home. After SCRIPT_WANDER_TRIES failed
/// tries it turns around and waits for the next frame.
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

  operands = &actor->script[(s16)actor->scriptCursor + 1];
  minimumDistance = *operands++;
  distanceRange = *operands++;
  homeRadiusSquared = *operands / FIXED_ONE;
  attempts = 0;
  homeRadiusSquared = homeRadiusSquared * homeRadiusSquared;
  for (;;) {
    attempts++;
    if (attempts <= SCRIPT_WANDER_TRIES) {
      SET_POSITION(&candidatePosition, actor->x.fixed, actor->y.fixed,
                   actor->z.fixed);
      travelDistance = minimumDistance + Iwram_MulFX(Random16(), distanceRange);
      heading = actor->facing + (Random16() >> 2) - (Random16() >> 2);
      Vector_AddPolarOffset(travelDistance, heading, &candidatePosition);
      if (ActorScript_CheckOverlap(actor, (s32 *)&candidatePosition) != 0 ||
          MapGrid_CheckCollision(actor, &candidatePosition) != 0)
        continue;
      travelDistance += SCRIPT_WANDER_PROBE_DISTANCE;
      SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed,
                   actor->z.fixed);
      // BUG: the probe straight ahead is placed but never tested, so only the
      // two at SCRIPT_WANDER_DIAGONAL_ANGLE need clear ground; SC_WANDER_HOME
      // tests all three.
      Vector_AddPolarOffset(travelDistance, heading, &probePosition);
      SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed,
                   actor->z.fixed);
      Vector_AddPolarOffset(travelDistance,
                            heading + SCRIPT_WANDER_DIAGONAL_ANGLE,
                            &probePosition);
      if (MapGrid_CheckCollision(actor, &probePosition) != 0)
        continue;
      SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed,
                   actor->z.fixed);
      Vector_AddPolarOffset(travelDistance,
                            heading - SCRIPT_WANDER_DIAGONAL_ANGLE,
                            &probePosition);
      if (MapGrid_CheckCollision(actor, &probePosition) != 0)
        continue;
      /* FAKEMATCH(dead): the offsets in both arms of a test of actor; as the
       * two lines alone. */
      if (actor != 0) {
        dx = candidatePosition.x / FIXED_ONE - actor->homeX;
        dz = candidatePosition.z / FIXED_ONE - actor->homeZ;
      } else {
        dx = candidatePosition.x / FIXED_ONE - actor->homeX;
        dz = candidatePosition.z / FIXED_ONE - actor->homeZ;
      }
      if (dx * dx + dz * dz > homeRadiusSquared)
        continue;
    } else {
      // Out of tries: face the other way and stay on this command, to try
      // again next frame.
      actor->facing += SCRIPT_HALF_TURN;
      actor->wait = 1;
      return 0;
    }
    Actor_SetMoveTarget(actor, candidatePosition.x, candidatePosition.y,
                        candidatePosition.z);
    actor->scriptCursor += 4;
    return 1;
  }
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

  operands = &actor->script[(s16)actor->scriptCursor + 1];
  minimumDistance = *operands++;
  distanceRange = *operands++;
  homeRadiusSquared = *operands / FIXED_ONE;
  angle = actor->facing;
  homeRadiusSquared *= homeRadiusSquared;
  attempts = 0;
  dx = actor->x.fixed / FIXED_ONE - actor->homeX;
  dz = actor->z.fixed / FIXED_ONE - actor->homeZ;
  if (dx * dx + dz * dz <= homeRadiusSquared) {
    for (;;) {
      attempts++;
      if (attempts <= SCRIPT_WANDER_TRIES) {
        travelDistance =
            minimumDistance + Iwram_MulFX(Random16(), distanceRange);
        heading = angle + (Random16() >> 2) - (Random16() >> 2);
        SET_POSITION(&candidatePosition, actor->x.fixed, actor->y.fixed,
                     actor->z.fixed);
        Vector_AddPolarOffset(SCRIPT_WANDER_PROBE_DISTANCE, heading,
                              &candidatePosition);
        if (ActorScript_CheckOverlap(actor, (s32 *)&candidatePosition) != 0)
          continue;
        SET_POSITION(&candidatePosition, actor->x.fixed, actor->y.fixed,
                     actor->z.fixed);
        Vector_AddPolarOffset(travelDistance, heading, &candidatePosition);
        if (MapGrid_CheckCollision(actor, &candidatePosition) != 0)
          continue;
        probePosition.x = actor->x.fixed;
        probePosition.y = actor->y.fixed;
        travelDistance += SCRIPT_WANDER_PROBE_DISTANCE;
        probePosition.z = actor->z.fixed;
        Vector_AddPolarOffset(travelDistance, heading, &probePosition);
        if (MapGrid_CheckCollision(actor, &probePosition) != 0)
          continue;
        SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed,
                     actor->z.fixed);
        Vector_AddPolarOffset(travelDistance,
                              heading + SCRIPT_WANDER_DIAGONAL_ANGLE,
                              &probePosition);
        if (MapGrid_CheckCollision(actor, &probePosition) != 0)
          continue;
        SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed,
                     actor->z.fixed);
        Vector_AddPolarOffset(travelDistance,
                              heading - SCRIPT_WANDER_DIAGONAL_ANGLE,
                              &probePosition);
        if (MapGrid_CheckCollision(actor, &probePosition) != 0)
          continue;
        SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed,
                     actor->z.fixed);
        Vector_AddPolarOffset(
            travelDistance, heading + SCRIPT_WANDER_SIDE_ANGLE, &probePosition);
        if (MapGrid_CheckCollision(actor, &probePosition) != 0)
          continue;
        SET_POSITION(&probePosition, actor->x.fixed, actor->y.fixed,
                     actor->z.fixed);
        Vector_AddPolarOffset(
            travelDistance, heading - SCRIPT_WANDER_SIDE_ANGLE, &probePosition);
        if (MapGrid_CheckCollision(actor, &probePosition) != 0)
          continue;
        dx = candidatePosition.x / FIXED_ONE - actor->homeX;
        dz = candidatePosition.z / FIXED_ONE - actor->homeZ;
        if (dx * dx + dz * dz > homeRadiusSquared)
          continue;
        actor->collisionFlags |= ACTOR_PUSHABLE;
        Actor_SetMoveTarget(actor, candidatePosition.x, candidatePosition.y,
                            candidatePosition.z);
        goto done;
      }
      break;
    }
  }
  attempts = 0;
  angle = (u16)Trig_Atan(dz, dx) + SCRIPT_HALF_TURN;
  for (;;) {
    attempts++;
    if (attempts <= SCRIPT_WANDER_TRIES) {
      travelDistance = minimumDistance + Iwram_MulFX(Random16(), distanceRange);
      heading = angle + (Random16() >> 2) - (Random16() >> 2);
      SET_POSITION(&candidatePosition, actor->x.fixed, actor->y.fixed,
                   actor->z.fixed);
      Vector_AddPolarOffset(SCRIPT_WANDER_PROBE_DISTANCE, heading,
                            &candidatePosition);
      if (ActorScript_CheckOverlap(actor, (s32 *)&candidatePosition) != 0)
        continue;
      SET_POSITION(&candidatePosition, actor->x.fixed, actor->y.fixed,
                   actor->z.fixed);
      Vector_AddPolarOffset(travelDistance, heading, &candidatePosition);
      if (MapGrid_CheckCollision(actor, &candidatePosition) != 0)
        continue;
      actor->collisionFlags &= ~ACTOR_PUSHABLE;
      Actor_SetMoveTarget(actor, candidatePosition.x, candidatePosition.y,
                          candidatePosition.z);
    }
    break;
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
/// gActorScript_OperandHandlerTable in actor_script_operands.c); each names the
/// actor field it sets, adds to or compares.
SCRIPT_DEFINE_OPERAND_ADDRESS(ActorScript_SetOrCompareScriptAddress, script)

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
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareBounce, bounce)
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

SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareTimer, timer, s16)
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareUserValue, userValue, s16)
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareWatchedActor, watchedActor)
SCRIPT_DEFINE_OPERAND_WORD(ActorScript_SetOrCompareUpdateAddress, update)
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareRiseCounter, riseCounter,
                             u8)
SCRIPT_DEFINE_OPERAND_NARROW(ActorScript_SetOrCompareRiseEnabled, riseEnabled,
                             u8)

/// Reads the two words after the command (a work value number and a value) and
/// passes operation to that value's handler.
static __inline__ s32
ActorScript_ApplyOperandCommand(struct ScriptOperands *operands,
                                s32 operation) {
  s16 commandCursor = operands->cursor;
  const s32 *arguments = (const s32 *)operands->script + commandCursor + 1;
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

/// Unused: sets an actor's update routine, when there is an actor.
UNUSED void Actor_SetUpdate(struct Actor *actor, void (*update)(void)) {
  if (actor != NULL)
    actor->update = update;
}

/// The heading of each direction-pad combination, -1 for none.
extern const s16 gWorldMap_KeyHeadings[16];

/// SC_HOVER: the leader hovering (the Hover Psynergy), moved by the direction
/// pad over what lies below.
s32 Actor_UpdateHoverControl(struct Actor *actor) {
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

  movementAnimation = ANIM_WALK;
  collision = 0;
  actor->motionFlags = ACTOR_FLOATS;
  // Hovering, the dash button changes only the animation, not the speed.
  if (gInput.held & gMainRoutineState.dashKeys) {
    actor->speedLimit = toFIXED(1) / 2;
    actor->acceleration = toFIXED(1) / 8;
    movementAnimation = ANIM_LEADER_RUN;
  } else {
    actor->speedLimit = toFIXED(1) / 2;
    actor->acceleration = toFIXED(1) / 8;
  }

  angle = gWorldMap_KeyHeadings[DPAD_INDEX(gInput.held)];
  if ((u16)angle == WORLD_MAP_KEY_HEADING_NONE) {
    collision |= PLAYER_STEP_STOPPED;
  } else {
    collision = 0;
    SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    Vector_AddPolarOffset(toFIXED(8), (u16)angle, &position);
    if (gDebugMode != 0 && (gInput.held & KEY_L)) {
      heading = angle;
    } else {
      if (MapGrid_CheckCollisionAbove(actor, &position) == 0) {
        SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed,
                     actor->z.fixed);
        Vector_AddPolarOffset(toFIXED(8), (u16)angle + FIELD_PROBE_ANGLE,
                              &testPosition);
        if (MapGrid_CheckCollisionAbove(actor, &testPosition) == 0) {
          SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed,
                       actor->z.fixed);
          Vector_AddPolarOffset(toFIXED(8), (u16)angle - FIELD_PROBE_ANGLE,
                                &testPosition);
          if (MapGrid_CheckCollisionAbove(actor, &testPosition) == 0) {
            SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed,
                         actor->z.fixed);
            Vector_AddPolarOffset(toFIXED(8), (u16)angle + DEGtoANG(45),
                                  &testPosition);
            if (MapGrid_CheckCollisionAbove(actor, &testPosition) == 0) {
              SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed,
                           actor->z.fixed);
              Vector_AddPolarOffset(toFIXED(8), (u16)angle - DEGtoANG(45),
                                    &testPosition);
              if (MapGrid_CheckCollisionAbove(actor, &testPosition) == 0) {
                heading = angle;
                goto check_objects;
              }
            }
          }
        }
      }
      direction = angle;
      headingCount = 0;
      angleOffsets[headingCount++] = direction + FIELD_PROBE_ANGLE;
      angleOffsets[headingCount++] = direction - FIELD_PROBE_ANGLE;
      angleOffsets[headingCount++] = direction + DEGtoANG(45);
      angleOffsets[headingCount++] = direction - DEGtoANG(45);
      angleOffsets[headingCount++] = direction + FIELD_PROBE_ANGLE * 3;
      angleOffsets[headingCount++] = direction - FIELD_PROBE_ANGLE * 3;
      for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
        heading = angleOffsets[angleIndex];
        SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
        Vector_AddPolarOffset(toFIXED(8), (u16)heading, &position);
        if (MapGrid_CheckCollisionAbove(actor, &position) != 0)
          continue;
        SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed,
                     actor->z.fixed);
        Vector_AddPolarOffset(toFIXED(8), (u16)heading + FIELD_PROBE_ANGLE,
                              &testPosition);
        if (MapGrid_CheckCollisionAbove(actor, &testPosition) != 0)
          continue;
        SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed,
                     actor->z.fixed);
        Vector_AddPolarOffset(toFIXED(8), (u16)heading - FIELD_PROBE_ANGLE,
                              &testPosition);
        if (MapGrid_CheckCollisionAbove(actor, &testPosition) != 0)
          continue;
        SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed,
                     actor->z.fixed);
        Vector_AddPolarOffset(toFIXED(8), (u16)heading + DEGtoANG(45),
                              &testPosition);
        if (MapGrid_CheckCollisionAbove(actor, &testPosition) != 0)
          continue;
        SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed,
                     actor->z.fixed);
        Vector_AddPolarOffset(toFIXED(8), (u16)heading - DEGtoANG(45),
                              &testPosition);
        if (MapGrid_CheckCollisionAbove(actor, &testPosition) == 0)
          goto check_objects;
      }
      SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
      collision |= PLAYER_STEP_BLOCKED_BY_MAP;

    check_objects:
      SET_POSITION(&contact, actor->x.fixed, actor->y.fixed, actor->z.fixed);
      Vector_AddPolarOffset(toFIXED(4), (u16)heading, &contact);
      other = Ram_HeapSlots->actors;
      for (angleIndex = 0; angleIndex < ACTOR_COUNT; angleIndex++, other++) {
        s32 radius;

        radius = actor->radius - 2;
        if (other->script == 0)
          continue;
        if ((other->collisionFlags & ACTOR_SOLID) == 0)
          continue;
        if (other == actor)
          continue;
        if (Field_CheckSpheresOverlap(&other->x.fixed, radius, &contact.x,
                                      other->radius - 2) < 0)
          continue;
        collision |= PLAYER_STEP_BLOCKED_BY_ACTOR;
      }
    }
  }

  if (Ram_HeapSlots->fieldState != NULL) {
    struct FieldState *state = Ram_HeapSlots->fieldState;

    if (collision & PLAYER_STEP_BLOCKED)
      state->blockedSteps++;
    else
      state->blockedSteps = 0;
  }
  if (collision != 0) {
    s32 action;

    action = ANIM_LEADER_LOOK_AROUND;
    if (Unit_Get(gMainRoutineState.leaderActorId)->hp == 0)
      action = ANIM_LEADER_LEAN_BACK;
    Actor_SelectAnimation(actor, action);
  } else {
    Actor_SelectAnimation(actor, movementAnimation);
  }

  if (collision != 0) {
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
    if (collision & PLAYER_STEP_BLOCKED) {
      s32 difference;

      difference = (u16)angle - actor->facing;
      difference = (s16)difference;
      if (difference > FIELD_BLOCKED_TURN_MAX)
        difference = FIELD_BLOCKED_TURN_MAX;
      if (difference < -FIELD_BLOCKED_TURN_MAX)
        difference = -FIELD_BLOCKED_TURN_MAX;
      actor->facing += difference;
    }
    actor->timer = 0;
    actor->userValue = 2;
  } else {
    Actor_SetMoveTarget(actor, position.x, position.y, position.z);
    if (actor->timer != 0)
      actor->timer--;
  }

  pressed = 0;
  if (Ram_HeapSlots->fieldState == NULL)
    return 0;
  if ((gInput.pressed & gMainRoutineState.selectKeys) ||
      (gInput.pressed & gMainRoutineState.aKeys) ||
      (gInput.pressed & gMainRoutineState.startKeys) ||
      (gInput.pressed & gMainRoutineState.firstShortcutKeys) ||
      (gInput.pressed & gMainRoutineState.secondShortcutKeys))
    pressed = 1;
  // While hovering the menus and shortcuts are refused with a buzz.
  if (pressed) {
    Sound_PlayFar(SE_MENU_CANCEL);
  } else {
    actor->groundHeight =
        MapGrid_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
    actor->scriptCursor++;
  }
  return 1;
}
#undef DISTANCE
#undef SCRIPT_LABEL_KEY_MASK
