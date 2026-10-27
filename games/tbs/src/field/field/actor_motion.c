/// @file
/// Actor motion, each frame, and the first actor script commands.
// Part of field.c.

#include "../field_internal.h"

/// SC_BASE: restarts the script at the next word, and returns 1 to run on.
s32 ActorScript_SetBase(struct Actor *interpreter) {
  s32 zero;

  // The word after the command becomes the script's start.
  interpreter->script = interpreter->script + interpreter->scriptCursor + 1;
  zero = 0;
  interpreter->scriptCursor = zero;
  return 1;
}

/// SC_HIDE_SPRITES: clears spriteStorageKind, hiding the sprites, and moves on
/// to the next command.
s32 ActorScript_HideSprites(struct Actor *interpreter) {
  interpreter->spriteStorageKind = ACTOR_SPRITE_NONE;
  interpreter->scriptCursor++;
  return 1;
}

/// SC_SHOW_SPRITE: sets spriteStorageKind to ACTOR_SPRITE_ONE, one sprite
/// drawn, and moves on to the next command.
s32 ActorScript_ShowSprite(struct Actor *interpreter) {
  interpreter->spriteStorageKind = ACTOR_SPRITE_ONE;
  interpreter->scriptCursor++;
  return 1;
}

/// Waits a frame at a time until an actor has arrived at its target, at
/// most 600 frames (ten seconds), after which it returns with the actor still
/// on its way. It takes void * because callers pass every view of an
/// actor. Called through Actor_WaitForMoveFar by field Psynergy
/// and by about 50 scene overlays.
void Actor_WaitForMove(void *actor) {
  s32 waitedFrames;

  waitedFrames = 0;
  while (waitedFrames < 600 && Actor_IsTargetUnset(actor) == 0) {
    WaitTicks(1);
    waitedFrames++;
  }
}

/// Returns 1 when the actor has no movement target: none of its target
/// coordinates set, or for an actor moved by the ground (any motion flag)
/// neither its target x nor z. Called by Actor_WaitForMove and SC_SYNC
/// (ActorScript_WaitForArrival).
s32 Actor_IsTargetUnset(struct Actor *actor) {
  s32 first;
  s32 second;

  // One compare of three values: all must equal TARGET_UNSET, or with
  // motion flags only x and z.
  if (actor->motionFlags == 0) {
    second = actor->targetX;
    if (second != TARGET_UNSET)
      return 0;
    first = actor->targetY;
  } else {
    first = actor->targetX;
    second = TARGET_UNSET;
  }
  if (first != second || actor->targetZ != first)
    return 0;
  return 1;
}

static __inline__ s32 Actor_Square(s32 value) { return value * value; }

/// Advances all 64 actors by one frame, as Actor_UpdateAll does for
/// the eight script-driven actors, with the field's extras: free flight in 3D,
/// following the ground, floating, and solid actors that refuse to overlap.
/// ActorSystem_Initialize adds it as the task that updates the actors in
/// every mode but the battle's (4). Writes each live actor's position,
/// velocity, target, wait and facing.
void Actor_UpdateAllThumb(void) {
  struct Actor *actor;
  const ActorScriptWords *script;
  s32 objectsRemaining;
  s32 x;
  s32 y;
  s32 z;
  s32 ground;
  s32 arrivedAtTarget;
  s32 pos[3];
  s32 dx;
  s32 dy;
  s32 dz;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 distance;
  s32 speed;
  s32 ratio;
  s32 reducedSpeed;
  s32 maxTerrainChange;
  s32 facingStep;
  u32 command;
  u32 phase;

  actor = gActors;
  for (objectsRemaining = ACTOR_COUNT - 1; objectsRemaining >= 0;
       objectsRemaining--, actor++) {
    arrivedAtTarget = 0;

    // The script indexed as words, which compiles as the old word-array
    // member did.
    script = (const ActorScriptWords *)actor->script;
    if (script == NULL)
      continue;

    // The update routine runs even for a paused actor, and may release it.
    if (actor->update != NULL) {
      actor->update((struct Actor *)actor);
      script = (const ActorScriptWords *)actor->script;
    }
    if (script == NULL)
      continue;
    if (actor->paused != 0)
      continue;

    // Count down a wait, or run commands until one yields for this frame.
    // Words above 63 are labels and are stepped over; a command may replace
    // the script, so it is read again after each one.
    if (actor->wait != 0) {
      actor->wait--;
    } else {
      for (;;) {
#ifdef UBFIX
        command = script[(s16)actor->scriptCursor];
#else
        command = script->word[(s16)actor->scriptCursor];
#endif
        if (command <= 63) {
          if (gActorScript_CommandTable[command]((struct Actor *)actor) == 0)
            break;
        } else {
          actor->scriptCursor++;
        }
        script = (const ActorScriptWords *)actor->script;
      }
      if (actor->script == NULL)
        continue;
    }

    x = actor->x.fixed;
    y = actor->y.fixed;
    z = actor->z.fixed;

    if (actor->frozen == 0) {
      // Free flight (no motion flags): steer in 3D towards the target,
      // capped at the speed limit, or brake when there is none.
      if (actor->motionFlags == 0) {
        if (actor->targetX != TARGET_UNSET) {
          dx = (actor->targetX - x) / 65536;
          dy = (actor->targetY - y) / 65536;
          dz = (actor->targetZ - z) / 65536;
          distance = Iwram_Sqrt(Actor_Square(dx) + Actor_Square(dy) +
                                Actor_Square(dz));
          if (distance == 0) {
            x = actor->targetX;
            y = actor->targetY;
            z = actor->targetZ;
          } else {
            // ratio is acceleration / distance in 16.16, and dx, dy and dz
            // are whole pixels, so each axis gains its share of the
            // acceleration toward the target.
            ratio = Iwram_RatioMulQ14(distance << 16, actor->acceleration);
            actor->velocityX += dx * ratio;
            actor->velocityY += dy * ratio;
            actor->velocityZ += dz * ratio;
            velocityX = actor->velocityX;
            velocityY = actor->velocityY;
            velocityZ = actor->velocityZ;
            speed = FixedSqrt(Iwram_MulQ16(velocityX, velocityX) +
                              Iwram_MulQ16(velocityY, velocityY) +
                              Iwram_MulQ16(velocityZ, velocityZ));
            // Too fast: scale the velocity by speedLimit / speed.
            if (speed > actor->speedLimit) {
              ratio = Iwram_RatioMulQ14(speed, actor->speedLimit);
              SET_VELOCITY(actor, Iwram_MulQ16(velocityX, ratio),
                           Iwram_MulQ16(velocityY, ratio),
                           Iwram_MulQ16(velocityZ, ratio));
            }
          }
        } else {
          // No target: take acceleration off the speed each frame, keeping
          // the direction, until it stops.
          dx = actor->velocityX;
          dy = actor->velocityY;
          dz = actor->velocityZ;
          speed = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                            Iwram_MulQ16(dz, dz));
          if (speed != 0) {
            reducedSpeed = speed - actor->acceleration;
            if (reducedSpeed < 0)
              reducedSpeed = 0;
            ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
            SET_VELOCITY(actor, Iwram_MulQ16(dx, ratio),
                         Iwram_MulQ16(dy, ratio), Iwram_MulQ16(dz, ratio));
          } else {
            SET_VELOCITY(actor, 0, 0, 0);
          }
        }
      } else {
        // On the ground: steer in the XZ plane the same way.
        if (actor->targetX != TARGET_UNSET) {
          dx = (actor->targetX - x) / 65536;
          dz = (actor->targetZ - z) / 65536;
          distance = Iwram_Sqrt(Actor_Square(dx) + Actor_Square(dz)) << 16;
          // Under 256 pixels, measure again in 16.16 for sub-pixel accuracy;
          // further away the squares would overflow.
          if (distance <= 0x00ffffff) {
            dx = actor->targetX - x;
            dz = actor->targetZ - z;
            distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
          }
          if (distance == 0) {
            x = actor->targetX;
            z = actor->targetZ;
          } else {
            ratio = Iwram_RatioMulQ14(distance, actor->acceleration);
            actor->velocityX += Iwram_MulQ16(dx, ratio);
            actor->velocityZ += Iwram_MulQ16(dz, ratio);
            velocityX = actor->velocityX;
            velocityZ = actor->velocityZ;
            speed = FixedSqrt(Iwram_MulQ16(velocityX, velocityX) +
                              Iwram_MulQ16(velocityZ, velocityZ));
            if (speed > actor->speedLimit) {
              ratio = Iwram_RatioMulQ14(speed, actor->speedLimit);
              actor->velocityX = Iwram_MulQ16(velocityX, ratio);
              actor->velocityZ = Iwram_MulQ16(velocityZ, ratio);
            }
          }
        } else {
          dx = actor->velocityX;
          dz = actor->velocityZ;
          speed = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
          if (speed != 0) {
            reducedSpeed = speed - actor->acceleration;
            if (reducedSpeed < 0)
              reducedSpeed = 0;
            ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
            actor->velocityX = Iwram_MulQ16(dx, ratio);
            actor->velocityZ = Iwram_MulQ16(dz, ratio);
          } else {
            actor->velocityX = 0;
            actor->velocityZ = 0;
          }
        }

        // Follow the ground: take the ground height where the actor is
        // going and move with its change, unless the actor is 4 or more
        // pixels above the ground. Unless bit 4 is set, a climb or drop also
        // slows it, by three times the change (at most half its
        // acceleration).
        if ((actor->motionFlags & ACTOR_FOLLOWS_TERRAIN) != 0) {
          velocityX = x + actor->velocityX;
          velocityZ = z + actor->velocityZ;
          ground = MapGrid_GetTerrainHeight(actor->layer, velocityX, velocityZ);
          velocityY = ground - actor->groundHeight;
          if (ground - y > -0x40000)
            y += velocityY;
          if (velocityY < 0)
            velocityY = -velocityY;
          maxTerrainChange = actor->acceleration / 2;
          if (velocityY > maxTerrainChange)
            velocityY = maxTerrainChange;
          velocityY *= 3;
          if (velocityY != 0 &&
              (actor->motionFlags & ACTOR_KEEPS_SPEED_ON_SLOPES) == 0) {
            dx = actor->velocityX;
            dy = actor->velocityY;
            dz = actor->velocityZ;
            speed = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                              Iwram_MulQ16(dz, dz));
            if (speed != 0) {
              reducedSpeed = speed - velocityY;
              if (reducedSpeed < 0)
                reducedSpeed = 0;
              ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
              SET_VELOCITY(actor, Iwram_MulQ16(dx, ratio),
                           Iwram_MulQ16(dy, ratio), Iwram_MulQ16(dz, ratio));
            }
          }
          actor->groundHeight = ground;
        }

        // Gravity, and a bounce that loses energy on landing.
        if ((actor->motionFlags & ACTOR_FALLS) != 0) {
          ground = actor->groundHeight;
          if (y > ground) {
            actor->velocityY -= actor->gravity;
          } else if (actor->velocityY < 0) {
            y = ground;
            actor->velocityY = -Iwram_MulQ16(actor->velocityY, actor->bounce);
            // A rebound no faster than one frame of gravity ends the bouncing.
            if ((actor->velocityY < 0 ? -actor->velocityY : actor->velocityY) <=
                actor->gravity)
              actor->velocityY = 0;
          }
        }

        // Float: bob up and down on a triangle wave (ACTOR_FLOATS_HIGH
        // makes it bigger). The phase runs over 64 frames, each table entry
        // held for two. The first 32 frames' velocities sum to 128 * gravity /
        // 64, so the actor rises and falls by about 2 * gravity pixels, or 8 *
        // gravity with ACTOR_FLOATS_HIGH (2 and 8 pixels at the default
        // gravity of 1.0).
        if ((actor->motionFlags & ACTOR_FLOATS) != 0) {
          phase = (actor->bounce & 0x3f);
          if ((actor->motionFlags & ACTOR_FLOATS_HIGH) != 0)
            actor->velocityY =
                gActorScript_CycleTable[phase >> 1] * actor->gravity / 16;
          else
            actor->velocityY =
                gActorScript_CycleTable[phase >> 1] * actor->gravity / 64;
          actor->bounce++;
        }
      }
    }

    x += actor->velocityX;
    y += actor->velocityY;
    z += actor->velocityZ;

    // An actor with ACTOR_BLOCKED_BY_SOLID does not move into an
    // ACTOR_SOLID one; count the refusals. A refused actor keeps last
    // frame's position and skips arrival and turning; SC_SYNC stops waiting for
    // it after 60.
    if ((actor->collisionFlags & ACTOR_BLOCKED_BY_SOLID) != 0) {
      SET_VECTOR(pos, x, y, z);
      if (ActorScript_CheckOverlap(actor, pos) != 0) {
        actor->refused++;
        continue;
      }
      actor->refused = 0;
    }

    // Arrived: the watched axis reached the target, or crossed it.
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
    default:
      break;
    }

    if (arrivedAtTarget != 0) {
      if (actor->snapToTarget != 0) {
        x = actor->targetX;
        actor->velocityX = 0;
        z = actor->targetZ;
        actor->velocityZ = 0;
        if (actor->motionFlags == 0) {
          y = actor->targetY;
          actor->velocityY = 0;
        }
      }
      actor->targetX = TARGET_UNSET;
      actor->targetY = TARGET_UNSET;
      actor->targetZ = TARGET_UNSET;
      actor->arrivalAxis = 0;
    }

    SET_ACTOR_POSITION(actor, x, y, z);

    // Turn to face the direction of travel, at most a sixteenth of a turn
    // (0x1000) a frame.
    if ((actor->steeringFlags & 1) != 0) {
      x = actor->velocityX;
      z = actor->velocityZ;
      if (x != 0 || z != 0) {
        s32 maximumFacingStep;

        // As a signed halfword the difference is the shorter way round.
        facingStep = (s16)(ArcTan2(z, x) - actor->facing);
        maximumFacingStep = FACING_STEP;
        if (facingStep > maximumFacingStep)
          facingStep = maximumFacingStep;
        maximumFacingStep = -0x1000;
        if (facingStep < maximumFacingStep)
          facingStep = maximumFacingStep;
        actor->facing += facingStep;
      }
    }
  }
}

/// Places the actor at (x, y, z), in 16.16 pixels, clears its movement target
/// and stops it. Reached through SC_POSITION and Actor_Create, and as
/// Actor_SetPositionAndResetMotionFar from field Psynergy and the
/// overworld's actor code.
void Actor_SetPositionAndResetMotion(struct Actor *actor, s32 x, s32 y, s32 z) {
  actor->z.fixed = z;
  actor->targetX = TARGET_UNSET;
  actor->targetY = TARGET_UNSET;
  actor->targetZ = TARGET_UNSET;
  actor->x.fixed = x;
  actor->y.fixed = y;
  SET_VELOCITY(actor, 0, 0, 0);
}

/// Sends an actor towards (x, y, z), in 16.16 pixels. Within a pixel it
/// arrives at once. Unless it snaps to its target, the target is pulled in
/// by half its braking distance (speed limit squared over acceleration), or
/// to halfway when that is further, as the actor coasts on. Picks the
/// axis to watch for arrival: the one with the furthest to go (y only for a
/// free-flying actor).
///
/// Reached through SC_MOVE and the other move commands, the player control,
/// and as Actor_SetMoveTargetFar from the battle's motion, the battle
/// animations and field Psynergy. Sets the target and arrivalAxis.
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z) {
  s32 dx;
  s32 distance;
  s32 dz;
  s32 squaredZ;
  u8 *arrivalAxis;

  dx = (x - actor->x.fixed) / Q16_ONE;
  distance = (y - actor->y.fixed) / Q16_ONE;
  dz = (z - actor->z.fixed) / Q16_ONE;
  squaredZ = dz * dz;
  distance = Iwram_Sqrt(dx * dx + distance * distance + squaredZ) << 16;
  // Under 16 whole pixels, measure again in 16.16 for sub-pixel accuracy.
  if (distance < 0x100000) {
    dx = x - actor->x.fixed;
    distance = y - actor->y.fixed;
    dz = z - actor->z.fixed;
    distance =
        FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(distance, distance) +
                  Iwram_MulQ16(dz, dz));
  }
  if (distance < Q16_ONE) {
    SET_ACTOR_POSITION(actor, x, y, z);
    actor->targetX = TARGET_UNSET;
    actor->targetY = TARGET_UNSET;
    actor->targetZ = TARGET_UNSET;
    return;
  }
  if (actor->snapToTarget == 0) {
    s32 brakingDistance;

    // speedLimit squared over acceleration, in 16.16 pixels: twice the
    // distance an actor at its limit coasts while it brakes to a stop, so
    // the shortened target leaves it room to coast onto the point asked for.
    brakingDistance =
        Iwram_RatioMulQ14(actor->acceleration,
                          Iwram_MulQ16(actor->speedLimit, actor->speedLimit));
    if (distance > brakingDistance)
      brakingDistance = distance - brakingDistance / 2;
    else
      brakingDistance = distance / 2;
    // Scale the offset to the target by the shortened distance's share of
    // the whole.
    distance = Iwram_RatioMulQ14(distance, brakingDistance);
    x = actor->x.fixed + Iwram_MulQ16(x - actor->x.fixed, distance);
    y = actor->y.fixed + Iwram_MulQ16(y - actor->y.fixed, distance);
    z = actor->z.fixed + Iwram_MulQ16(z - actor->z.fixed, distance);
  }
  actor->targetX = x;
  actor->targetY = y;
  actor->targetZ = z;
  // Watch the axis with the furthest to go: a crossing on an axis with
  // little to go can come long before the target is reached. y only counts
  // in free flight (no motion flags); otherwise the ground sets the height.
  dx = x - actor->x.fixed;
  distance = y - actor->y.fixed;
  dz = z - actor->z.fixed;
  arrivalAxis = &actor->arrivalAxis;
  *arrivalAxis = ARRIVAL_AXIS_X;
  if ((dx < 0 ? -dx : dx) < (dz < 0 ? -dz : dz)) {
    *arrivalAxis = ARRIVAL_AXIS_Z;
    dx = dz;
  }
  if (actor->motionFlags == 0) {
    if (dx < 0)
      dx = -dx;
    if (dx < (distance < 0 ? -distance : distance))
      *arrivalAxis = ARRIVAL_AXIS_Y;
  }
}

/// Unused: copies Actor_UpdateAll (ARM code) to the heap, runs it once and
/// frees it.
UNUSED void Actor_RunUpdateAllFromHeap(void) {
  void (*updateActors)(void);

  RamCode_Load(updateActors, Actor_UpdateAll, Actor_UpdateAllCodeSize);
  updateActors();
  Free(updateActors);
}

/// Advances the first 14 actors' motion by one frame, without running their
/// scripts: steering towards a target or braking, gravity and bouncing,
/// arrival, and turning to face the direction of travel.
/// ActorSystem_Initialize adds it as a task in place of Actor_UpdateAllThumb
/// in ACTOR_SYSTEM_BATTLE.
void Actor_UpdateAllMotion(void) {
  struct Actor *actor;
  s32 objectsRemaining;
  s32 y;
  s32 arrivedAtTarget;
  s32 x;
  s32 z;
  s32 dx;
  s32 dz;
  s32 velocityX;
  s32 distance;
  s32 ratio;
  s32 facingStep;

  // Fourteen actors, as many as the battle has units (UNIT_MAX in
  // battle/battle/objects.c).
  actor = gActors;
  for (objectsRemaining = 13; objectsRemaining >= 0;
       objectsRemaining--, actor++) {
    if (actor->script == 0)
      continue;
    x = actor->x.fixed;
    y = actor->y.fixed;
    z = actor->z.fixed;
    // UB: a frozen actor otherwise reads an uninitialized arrival flag, or
    // inherits the preceding actor's flag. Initializing it here changes the
    // EN load/store schedule while keeping the 788-byte function extent.
#ifdef UBFIX
    arrivedAtTarget = 0;
#endif
    if (actor->frozen == 0) {
#ifndef UBFIX
      arrivedAtTarget = 0;
#endif
      // Steer in the XZ plane as Actor_UpdateAllThumb does on the ground,
      // or brake; this loop has no free flight, ground following or floating.
      if (actor->targetX != TARGET_UNSET) {
        dx = (actor->targetX - x) / 65536;
        dz = (actor->targetZ - z) / 65536;
        distance = Iwram_Sqrt(dx * dx + dz * dz) << 16;
        // Under 256 pixels, measure again in 16.16: the root of a 16.16
        // square is 8.8, and << 8 makes it 16.16 again.
        if (distance <= 0xffffff) {
          dx = actor->targetX - x;
          dz = actor->targetZ - z;
          distance = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz))
                     << 8;
        }
        if (distance == 0) {
          x = actor->targetX;
          z = actor->targetZ;
        } else {
          ratio = Iwram_RatioMulQ14(distance, actor->acceleration);
          velocityX = actor->velocityX + Iwram_MulQ16(dx, ratio);
          actor->velocityX = velocityX;
          dx = actor->velocityZ + Iwram_MulQ16(dz, ratio);
          actor->velocityZ = dx;
          distance = Iwram_Sqrt(Iwram_MulQ16(velocityX, velocityX) +
                                Iwram_MulQ16(dx, dx))
                     << 8;
          if (distance > actor->speedLimit) {
            ratio = Iwram_RatioMulQ14(distance, actor->speedLimit);
            actor->velocityX = Iwram_MulQ16(velocityX, ratio);
            actor->velocityZ = Iwram_MulQ16(dx, ratio);
          }
        }
      } else {
        dx = actor->velocityX;
        dz = actor->velocityZ;
        if ((dx | dz) != 0) {
          distance = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz))
                     << 8;
          if (distance != 0) {
            if (distance - actor->acceleration < 0) {
              actor->velocityX = 0;
              actor->velocityZ = 0;
            } else {
              ratio =
                  Iwram_RatioMulQ14(distance, distance - actor->acceleration);
              actor->velocityX = Iwram_MulQ16(dx, ratio);
              actor->velocityZ = Iwram_MulQ16(dz, ratio);
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
          actor->velocityY = -Iwram_MulQ16(actor->velocityY, actor->bounce);
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
      actor->targetX = TARGET_UNSET;
      actor->targetY = TARGET_UNSET;
      actor->targetZ = TARGET_UNSET;
      actor->arrivalAxis = 0;
    }
    SET_ACTOR_POSITION(actor, x, y, z);
    // Turn to face the direction of travel, at most a sixteenth of a turn a
    // frame, the shorter way round.
    if (actor->steeringFlags & 1) {
      x = actor->velocityX;
      z = actor->velocityZ;
      if (x != 0 || z != 0) {
        facingStep = (s16)(ArcTan2(z, x) - actor->facing);
        if (facingStep > 0x1000)
          facingStep = FACING_STEP;
        if (facingStep < -0x1000)
          facingStep = -0x1000;
        actor->facing += facingStep;
      }
    }
  }
}
