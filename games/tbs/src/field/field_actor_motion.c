/// @file
/// Actor motion, each frame, and the first actor script commands.
// Part of field.c.

#include "field_internal.h"

/// SC_BASE: restarts the script at the next word, and returns 1 to run on.
s32 ActorScript_SetBase(struct Actor *interpreter) {
  interpreter->script = interpreter->script + interpreter->scriptCursor + 1;
  interpreter->scriptCursor = 0;
  return 1;
}

/// SC_HIDE_SPRITES: hides the actor's sprites and runs on.
s32 ActorScript_HideSprites(struct Actor *interpreter) {
  interpreter->spriteStorageKind = ACTOR_SPRITE_NONE;
  interpreter->scriptCursor++;
  return 1;
}

/// SC_SHOW_SPRITE: shows the actor's one sprite and runs on.
s32 ActorScript_ShowSprite(struct Actor *interpreter) {
  interpreter->spriteStorageKind = ACTOR_SPRITE_ONE;
  interpreter->scriptCursor++;
  return 1;
}

/// Waits a frame at a time until an actor has arrived at its target, at most
/// 600 frames (ten seconds), after which it returns with the actor still on its
/// way.
void Actor_WaitForMove(void *actor) {
  s32 waitedFrames;

  for (waitedFrames = 0; waitedFrames < 600 && Actor_IsTargetUnset(actor) == 0;
       waitedFrames++) {
    wait(1);
  }
}

/// Returns 1 when the actor has no movement target: none of its target
/// coordinates set, or for an actor moved by the ground (any motion flag)
/// neither its target x nor z.
s32 Actor_IsTargetUnset(struct Actor *actor) {
  s32 first;
  s32 second;

  // One compare of three values: all must equal ACTOR_NO_TARGET, or with
  // motion flags only x and z.
  if (actor->motionFlags == 0) {
    second = actor->targetX;
    if (second != ACTOR_NO_TARGET)
      return 0;
    first = actor->targetY;
  } else {
    first = actor->targetX;
    second = ACTOR_NO_TARGET;
  }
  if (first != second || actor->targetZ != first)
    return 0;
  return 1;
}

/// A floating actor's bob, one cycle of gActorScript_CycleTable over this many
/// frames; its rise and fall each frame is the table's value times its gravity
/// over FLOAT_DIVISOR, or FLOAT_HIGH_DIVISOR with ACTOR_FLOATS_HIGH.
#define FLOAT_CYCLE_FRAMES 64
#define FLOAT_DIVISOR 64
#define FLOAT_HIGH_DIVISOR 16

static __inline__ s32 Actor_Square(s32 value) { return value * value; }

/// Advances all 64 actors by one frame, as Actor_UpdateAll does for the eight
/// script-driven actors, with the field's extras: free flight in 3D, following
/// the ground, floating, and solid actors that refuse to overlap.
void Actor_UpdateAllOnField(void) {
  struct Actor *actor;
  const ActorScriptWords *script;
  s32 actorsLeft;
  FIXED x;
  FIXED y;
  FIXED z;
  FIXED ground;
  s32 arrivedAtTarget;
  s32 pos[3];
  s32 dx;
  s32 dy;
  s32 dz;
  FIXED velocityX;
  s32 velocityY;
  FIXED velocityZ;
  s32 distance;
  FIXED speed;
  FIXED ratio;
  FIXED reducedSpeed;
  s32 maxTerrainChange;
  s32 facingStep;
  u32 command;
  u32 phase;

  actor = gActors;
  for (actorsLeft = ACTOR_COUNT - 1; actorsLeft >= 0; actorsLeft--, actor++) {
    arrivedAtTarget = 0;

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

    // Count down a wait, or run commands until one waits for this frame.
    if (actor->wait != 0) {
      actor->wait--;
    } else {
      for (;;) {
#ifdef UBFIX
        command = script[actor->scriptCursor];
#else
        command = script->word[actor->scriptCursor];
#endif
        if (command <= SCRIPT_COMMAND_MAX) {
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
        if (actor->targetX != ACTOR_NO_TARGET) {
          dx = (actor->targetX - x) / FIXED_ONE;
          dy = (actor->targetY - y) / FIXED_ONE;
          dz = (actor->targetZ - z) / FIXED_ONE;
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
            ratio = Iwram_RatioMulQ14(toFIXED(distance), actor->acceleration);
            actor->velocityX += dx * ratio;
            actor->velocityY += dy * ratio;
            actor->velocityZ += dz * ratio;
            velocityX = actor->velocityX;
            velocityY = actor->velocityY;
            velocityZ = actor->velocityZ;
            speed = Math_SqrtFX(Iwram_MulFX(velocityX, velocityX) +
                                Iwram_MulFX(velocityY, velocityY) +
                                Iwram_MulFX(velocityZ, velocityZ));
            // Too fast: scale the velocity by speedLimit / speed.
            if (speed > actor->speedLimit) {
              ratio = Iwram_RatioMulQ14(speed, actor->speedLimit);
              SET_VELOCITY(actor, Iwram_MulFX(velocityX, ratio),
                           Iwram_MulFX(velocityY, ratio),
                           Iwram_MulFX(velocityZ, ratio));
            }
          }
        } else {
          // No target: take acceleration off the speed each frame, keeping
          // the direction, until it stops.
          dx = actor->velocityX;
          dy = actor->velocityY;
          dz = actor->velocityZ;
          speed = Math_SqrtFX(Iwram_MulFX(dx, dx) + Iwram_MulFX(dy, dy) +
                              Iwram_MulFX(dz, dz));
          if (speed != 0) {
            reducedSpeed = speed - actor->acceleration;
            if (reducedSpeed < 0)
              reducedSpeed = 0;
            ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
            SET_VELOCITY(actor, Iwram_MulFX(dx, ratio), Iwram_MulFX(dy, ratio),
                         Iwram_MulFX(dz, ratio));
          } else {
            SET_VELOCITY(actor, 0, 0, 0);
          }
        }
      } else {
        // On the ground: steer in the XZ plane the same way.
        if (actor->targetX != ACTOR_NO_TARGET) {
          dx = (actor->targetX - x) / FIXED_ONE;
          dz = (actor->targetZ - z) / FIXED_ONE;
          distance = toFIXED(Iwram_Sqrt(Actor_Square(dx) + Actor_Square(dz)));
          // Under 256 pixels, measure again in 16.16 for sub-pixel accuracy;
          // further away the squares would overflow.
          if (distance <= toFIXED(256) - 1) {
            dx = actor->targetX - x;
            dz = actor->targetZ - z;
            distance = Math_SqrtFX(Iwram_MulFX(dx, dx) + Iwram_MulFX(dz, dz));
          }
          if (distance == 0) {
            x = actor->targetX;
            z = actor->targetZ;
          } else {
            ratio = Iwram_RatioMulQ14(distance, actor->acceleration);
            actor->velocityX += Iwram_MulFX(dx, ratio);
            actor->velocityZ += Iwram_MulFX(dz, ratio);
            velocityX = actor->velocityX;
            velocityZ = actor->velocityZ;
            speed = Math_SqrtFX(Iwram_MulFX(velocityX, velocityX) +
                                Iwram_MulFX(velocityZ, velocityZ));
            if (speed > actor->speedLimit) {
              ratio = Iwram_RatioMulQ14(speed, actor->speedLimit);
              actor->velocityX = Iwram_MulFX(velocityX, ratio);
              actor->velocityZ = Iwram_MulFX(velocityZ, ratio);
            }
          }
        } else {
          dx = actor->velocityX;
          dz = actor->velocityZ;
          speed = Math_SqrtFX(Iwram_MulFX(dx, dx) + Iwram_MulFX(dz, dz));
          if (speed != 0) {
            reducedSpeed = speed - actor->acceleration;
            if (reducedSpeed < 0)
              reducedSpeed = 0;
            ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
            actor->velocityX = Iwram_MulFX(dx, ratio);
            actor->velocityZ = Iwram_MulFX(dz, ratio);
          } else {
            actor->velocityX = 0;
            actor->velocityZ = 0;
          }
        }

        // Follow the ground: take the ground height where the actor is going
        // and move with its change, unless the actor is 4 or more pixels above
        // the ground.
        if ((actor->motionFlags & ACTOR_FOLLOWS_TERRAIN) != 0) {
          velocityX = x + actor->velocityX;
          velocityZ = z + actor->velocityZ;
          ground = MapGrid_GetTerrainHeight(actor->layer, velocityX, velocityZ);
          velocityY = ground - actor->groundHeight;
          if (ground - y > -toFIXED(4))
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
            speed = Math_SqrtFX(Iwram_MulFX(dx, dx) + Iwram_MulFX(dy, dy) +
                                Iwram_MulFX(dz, dz));
            if (speed != 0) {
              reducedSpeed = speed - velocityY;
              if (reducedSpeed < 0)
                reducedSpeed = 0;
              ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
              SET_VELOCITY(actor, Iwram_MulFX(dx, ratio),
                           Iwram_MulFX(dy, ratio), Iwram_MulFX(dz, ratio));
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
            actor->velocityY = -Iwram_MulFX(actor->velocityY, actor->bounce);
            // A rebound no faster than one frame of gravity ends the bouncing.
            if ((actor->velocityY < 0 ? -actor->velocityY : actor->velocityY) <=
                actor->gravity)
              actor->velocityY = 0;
          }
        }

        // Float: bob up and down on a triangle wave (ACTOR_FLOATS_HIGH makes it
        // bigger).
        if ((actor->motionFlags & ACTOR_FLOATS) != 0) {
          phase = actor->bounce & (FLOAT_CYCLE_FRAMES - 1);
          if ((actor->motionFlags & ACTOR_FLOATS_HIGH) != 0)
            actor->velocityY = gActorScript_CycleTable[phase >> 1] *
                               actor->gravity / FLOAT_HIGH_DIVISOR;
          else
            actor->velocityY = gActorScript_CycleTable[phase >> 1] *
                               actor->gravity / FLOAT_DIVISOR;
          actor->bounce++;
        }
      }
    }

    x += actor->velocityX;
    y += actor->velocityY;
    z += actor->velocityZ;

    // An actor with ACTOR_BLOCKED_BY_SOLID does not move into an ACTOR_SOLID
    // one; count the refusals.
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
      actor->targetX = ACTOR_NO_TARGET;
      actor->targetY = ACTOR_NO_TARGET;
      actor->targetZ = ACTOR_NO_TARGET;
      actor->arrivalAxis = ARRIVAL_AXIS_NONE;
    }

    SET_ACTOR_POSITION(actor, x, y, z);

    // Turn to face the direction of travel, at most FACING_STEP a frame.
    if ((actor->steeringFlags & ACTOR_FACES_MOTION) != 0) {
      x = actor->velocityX;
      z = actor->velocityZ;
      if (x != 0 || z != 0) {
        s32 maximumFacingStep;

        // As a signed halfword the difference is the shorter way round.
        facingStep = (s16)(Trig_Atan(z, x) - actor->facing);
        maximumFacingStep = FACING_STEP;
        if (facingStep > maximumFacingStep)
          facingStep = maximumFacingStep;
        maximumFacingStep = -FACING_STEP;
        if (facingStep < maximumFacingStep)
          facingStep = maximumFacingStep;
        actor->facing += facingStep;
      }
    }
  }
}

/// Places the actor at (x, y, z), in 16.16 pixels, clears its movement target
/// and stops it.
void Actor_SetPositionAndResetMotion(struct Actor *actor, FIXED x, FIXED y,
                                     FIXED z) {
  actor->z.fixed = z;
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  actor->x.fixed = x;
  actor->y.fixed = y;
  SET_VELOCITY(actor, 0, 0, 0);
}

/// Sends an actor towards (x, y, z), in 16.16 pixels. Within a pixel it arrives
/// at once.
void Actor_SetMoveTarget(struct Actor *actor, FIXED x, FIXED y, FIXED z) {
  s32 dx;
  s32 distance;
  s32 dz;
  s32 squaredZ;

  dx = (x - actor->x.fixed) / FIXED_ONE;
  distance = (y - actor->y.fixed) / FIXED_ONE;
  dz = (z - actor->z.fixed) / FIXED_ONE;
  squaredZ = dz * dz;
  distance = toFIXED(Iwram_Sqrt(dx * dx + distance * distance + squaredZ));
  // Under 16 whole pixels, measure again in 16.16 for sub-pixel accuracy.
  if (distance < toFIXED(16)) {
    dx = x - actor->x.fixed;
    distance = y - actor->y.fixed;
    dz = z - actor->z.fixed;
    distance =
        Math_SqrtFX(Iwram_MulFX(dx, dx) + Iwram_MulFX(distance, distance) +
                    Iwram_MulFX(dz, dz));
  }
  if (distance < FIXED_ONE) {
    SET_ACTOR_POSITION(actor, x, y, z);
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
    return;
  }
  if (actor->snapToTarget == 0) {
    s32 brakingDistance;

    // speedLimit squared over acceleration, in 16.16 pixels: twice the
    // distance an actor at its limit coasts while it brakes to a stop, so
    // the shortened target leaves it room to coast onto the point asked for.
    brakingDistance = Iwram_RatioMulQ14(
        actor->acceleration, Iwram_MulFX(actor->speedLimit, actor->speedLimit));
    if (distance > brakingDistance)
      brakingDistance = distance - brakingDistance / 2;
    else
      brakingDistance = distance / 2;
    // Scale the offset to the target by the shortened distance's share of
    // the whole.
    distance = Iwram_RatioMulQ14(distance, brakingDistance);
    x = actor->x.fixed + Iwram_MulFX(x - actor->x.fixed, distance);
    y = actor->y.fixed + Iwram_MulFX(y - actor->y.fixed, distance);
    z = actor->z.fixed + Iwram_MulFX(z - actor->z.fixed, distance);
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
  actor->arrivalAxis = ARRIVAL_AXIS_X;
  if ((dx < 0 ? -dx : dx) < (dz < 0 ? -dz : dz)) {
    actor->arrivalAxis = ARRIVAL_AXIS_Z;
    dx = dz;
  }
  if (actor->motionFlags == 0) {
    if (dx < 0)
      dx = -dx;
    if (dx < (distance < 0 ? -distance : distance))
      actor->arrivalAxis = ARRIVAL_AXIS_Y;
  }
}

/// Unused: copies Actor_UpdateAll (ARM code) to the heap, runs it once and
/// frees it.
UNUSED void Actor_RunUpdateAllFromHeap(void) {
  void (*updateActors)(void);

  RamCode_Load(updateActors, Actor_UpdateAll, Actor_UpdateAllCodeSize);
  updateActors();
  Heap_Release(updateActors);
}

/// Advances the battle's actors by one frame, without running their scripts:
/// steering towards a target or braking, gravity and bouncing, arrival, and
/// turning to face the direction of travel.
void Actor_UpdateAllMotion(void) {
  struct Actor *actor;
  s32 actorsLeft;
  FIXED y;
  s32 arrivedAtTarget;
  FIXED x;
  FIXED z;
  s32 dx;
  s32 dz;
  FIXED velocityX;
  s32 distance;
  FIXED ratio;
  s32 facingStep;

  // The first UNIT_MAX actors, as a battle makes one for each unit
  // (BattleObject_Make).
  actor = gActors;
  for (actorsLeft = 0; actorsLeft < UNIT_MAX; actorsLeft++, actor++) {
    if (actor->script == 0)
      continue;
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
      // Steer in the XZ plane as Actor_UpdateAllOnField does on the ground,
      // or brake; this loop has no free flight, ground following or floating.
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
          velocityX = actor->velocityX + Iwram_MulFX(dx, ratio);
          actor->velocityX = velocityX;
          dx = actor->velocityZ + Iwram_MulFX(dz, ratio);
          actor->velocityZ = dx;
          distance = Iwram_Sqrt(Iwram_MulFX(velocityX, velocityX) +
                                Iwram_MulFX(dx, dx))
                     << 8;
          if (distance > actor->speedLimit) {
            ratio = Iwram_RatioMulQ14(distance, actor->speedLimit);
            actor->velocityX = Iwram_MulFX(velocityX, ratio);
            actor->velocityZ = Iwram_MulFX(dx, ratio);
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
    if (actor->arrivalAxis != ARRIVAL_AXIS_NONE)
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
      actor->arrivalAxis = ARRIVAL_AXIS_NONE;
    }
    SET_ACTOR_POSITION(actor, x, y, z);
    // Turn to face the direction of travel, at most FACING_STEP a frame, the
    // shorter way round.
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
  }
}

#undef FLOAT_CYCLE_FRAMES
#undef FLOAT_DIVISOR
#undef FLOAT_HIGH_DIVISOR
