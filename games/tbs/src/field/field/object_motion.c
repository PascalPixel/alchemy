/// @file
/// Object motion, each frame, and the first object script commands.
// Part of field.c.

#include "../field_internal.h"

/// SC_BASE: restarts the script at the next word, and returns 1 to run on.
s32 ActionScript_SetBase(struct ScriptInterpreter *interpreter) {
  s32 zero;

  // The word after the command becomes the script's start.
  interpreter->script = interpreter->script + interpreter->cursor + 1;
  zero = 0;
  interpreter->cursor = zero;
  return 1;
}

/// SC_B54_OFF: clears spriteStorageKind, hiding the sprites, and moves on to
/// the next command.
s32 ActionScript_ClearByte54(struct ScriptInterpreter *interpreter) {
  interpreter->spriteStorageKind = OBJECT_SPRITE_NONE;
  interpreter->cursor++;
  return 1;
}

/// SC_B54_ON: sets spriteStorageKind to OBJECT_SPRITE_ONE, one sprite drawn,
/// and moves on to the next command.
s32 ActionScript_SetByte54(struct ScriptInterpreter *interpreter) {
  interpreter->spriteStorageKind = OBJECT_SPRITE_ONE;
  interpreter->cursor++;
  return 1;
}

/// Waits a frame at a time until an object event has arrived at its target, at
/// most 600 frames (ten seconds), after which it returns with the object still
/// on its way. It takes void * because callers pass every view of an
/// object. Called through FieldObject_WaitForMoveFar by the Psynergy effects
/// and by about 50 map overlays.
void FieldObject_WaitForMove(void *fieldObject) {
  s32 waitedFrames;

  waitedFrames = 0;
  while (waitedFrames < 600 && Object_IsTargetUnset(fieldObject) == 0) {
    WaitFrames(1);
    waitedFrames++;
  }
}

/// Returns 1 when the object has no movement target: none of its target
/// coordinates set, or for an object moved by the ground (any motion flag)
/// neither its target x nor z. Called by FieldObject_WaitForMove and SC_SYNC
/// (ActionScript_WaitForArrival).
s32 Object_IsTargetUnset(struct ObjectRuntime *object) {
  s32 first;
  s32 second;

  // One compare of three values: all must equal TARGET_UNSET, or with
  // motion flags only x and z.
  if (object->flags == 0) {
    second = object->targetX;
    if (second != TARGET_UNSET)
      return 0;
    first = object->targetY;
  } else {
    first = object->targetX;
    second = TARGET_UNSET;
  }
  if (first != second || object->targetZ != first)
    return 0;
  return 1;
}

static __inline__ s32 ObjectMotion_Square(s32 value) { return value * value; }

/// Advances all 64 object events by one frame, as Object_UpdateAll does for
/// the eight script objects, with the field's extras: free flight in 3D,
/// terrain following, floating, and solid objects that refuse to overlap.
/// ObjectSystem_Initialize schedules it as the per-frame object update in
/// every mode but the battle's (4). Writes each live object's position,
/// velocity, target, wait and facing.
void Object_UpdateAllThumb(void) {
  struct ScriptMotionObject *object;
#ifdef UBFIX
  const s32 *script;
#else
  const struct ScriptMotionWords *script;
#endif
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

  object = (struct ScriptMotionObject *)gObjectSlots;
  for (objectsRemaining = FIELD_OBJECT_COUNT - 1; objectsRemaining >= 0;
       objectsRemaining--, object++) {
    arrivedAtTarget = 0;

    script = object->script;
    if (script == NULL)
      continue;

    // The hook runs even for a paused object, and may free it.
    if (object->hook != NULL) {
      object->hook((union FieldObject *)object);
      script = object->script;
    }
    if (script == NULL)
      continue;
    if (object->paused != 0)
      continue;

    // Count down a wait, or run commands until one yields for this frame.
    // Words above 63 are labels and are stepped over; a command may replace
    // the script, so it is read again after each one.
    if (object->wait != 0) {
      object->wait--;
    } else {
      for (;;) {
#ifdef UBFIX
        command = script[(s16)object->scriptCursor];
#else
        command = script->word[(s16)object->scriptCursor];
#endif
        if (command <= 63) {
          if (gScriptObject_CommandTable[command](
                  (struct ScriptInterpreter *)object) == 0)
            break;
        } else {
          object->scriptCursor++;
        }
        script = object->script;
      }
      if (object->script == NULL)
        continue;
    }

    x = object->x;
    y = object->y;
    z = object->z;

    if (object->frozen == 0) {
      // Free flight (no motion flags): steer in 3D towards the target,
      // capped at the speed limit, or brake when there is none.
      if (object->motionFlags == 0) {
        if (object->targetX != TARGET_UNSET) {
          dx = (object->targetX - x) / 65536;
          dy = (object->targetY - y) / 65536;
          dz = (object->targetZ - z) / 65536;
          distance =
              Iwram_Sqrt(ObjectMotion_Square(dx) + ObjectMotion_Square(dy) +
                         ObjectMotion_Square(dz));
          if (distance == 0) {
            x = object->targetX;
            y = object->targetY;
            z = object->targetZ;
          } else {
            // ratio is acceleration / distance in 16.16, and dx, dy and dz
            // are whole pixels, so each axis gains its share of the
            // acceleration toward the target.
            ratio = Iwram_RatioMulQ14(distance << 16, object->acceleration);
            object->velocityX += dx * ratio;
            object->velocityY += dy * ratio;
            object->velocityZ += dz * ratio;
            velocityX = object->velocityX;
            velocityY = object->velocityY;
            velocityZ = object->velocityZ;
            speed = FixedSqrt(Iwram_MulQ16(velocityX, velocityX) +
                              Iwram_MulQ16(velocityY, velocityY) +
                              Iwram_MulQ16(velocityZ, velocityZ));
            // Too fast: scale the velocity by speedLimit / speed.
            if (speed > object->speedLimit) {
              ratio = Iwram_RatioMulQ14(speed, object->speedLimit);
              SET_VELOCITY(object, Iwram_MulQ16(velocityX, ratio),
                           Iwram_MulQ16(velocityY, ratio),
                           Iwram_MulQ16(velocityZ, ratio));
            }
          }
        } else {
          // No target: take acceleration off the speed each frame, keeping
          // the direction, until it stops.
          dx = object->velocityX;
          dy = object->velocityY;
          dz = object->velocityZ;
          speed = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                            Iwram_MulQ16(dz, dz));
          if (speed != 0) {
            reducedSpeed = speed - object->acceleration;
            if (reducedSpeed < 0)
              reducedSpeed = 0;
            ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
            SET_VELOCITY(object, Iwram_MulQ16(dx, ratio),
                         Iwram_MulQ16(dy, ratio), Iwram_MulQ16(dz, ratio));
          } else {
            SET_VELOCITY(object, 0, 0, 0);
          }
        }
      } else {
        // On the ground: steer in the XZ plane the same way.
        if (object->targetX != TARGET_UNSET) {
          dx = (object->targetX - x) / 65536;
          dz = (object->targetZ - z) / 65536;
          distance =
              Iwram_Sqrt(ObjectMotion_Square(dx) + ObjectMotion_Square(dz))
              << 16;
          // Under 256 pixels, measure again in 16.16 for sub-pixel accuracy;
          // further away the squares would overflow.
          if (distance <= 0x00ffffff) {
            dx = object->targetX - x;
            dz = object->targetZ - z;
            distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
          }
          if (distance == 0) {
            x = object->targetX;
            z = object->targetZ;
          } else {
            ratio = Iwram_RatioMulQ14(distance, object->acceleration);
            object->velocityX += Iwram_MulQ16(dx, ratio);
            object->velocityZ += Iwram_MulQ16(dz, ratio);
            velocityX = object->velocityX;
            velocityZ = object->velocityZ;
            speed = FixedSqrt(Iwram_MulQ16(velocityX, velocityX) +
                              Iwram_MulQ16(velocityZ, velocityZ));
            if (speed > object->speedLimit) {
              ratio = Iwram_RatioMulQ14(speed, object->speedLimit);
              object->velocityX = Iwram_MulQ16(velocityX, ratio);
              object->velocityZ = Iwram_MulQ16(velocityZ, ratio);
            }
          }
        } else {
          dx = object->velocityX;
          dz = object->velocityZ;
          speed = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
          if (speed != 0) {
            reducedSpeed = speed - object->acceleration;
            if (reducedSpeed < 0)
              reducedSpeed = 0;
            ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
            object->velocityX = Iwram_MulQ16(dx, ratio);
            object->velocityZ = Iwram_MulQ16(dz, ratio);
          } else {
            object->velocityX = 0;
            object->velocityZ = 0;
          }
        }

        // Follow the terrain: take the ground height where the object is
        // going and move with its change, unless the object is 4 or more
        // pixels above the ground. Unless bit 4 is set, a climb or drop also
        // slows it, by three times the change (at most half its
        // acceleration).
        if ((object->motionFlags & OBJ_EVENT_FOLLOWS_TERRAIN) != 0) {
          velocityX = x + object->velocityX;
          velocityZ = z + object->velocityZ;
          ground =
              MapGrid_GetTerrainHeight(object->terrainId, velocityX, velocityZ);
          velocityY = ground - object->terrainHeight;
          if (ground - y > -0x40000)
            y += velocityY;
          if (velocityY < 0)
            velocityY = -velocityY;
          maxTerrainChange = object->acceleration / 2;
          if (velocityY > maxTerrainChange)
            velocityY = maxTerrainChange;
          velocityY *= 3;
          if (velocityY != 0 &&
              (object->motionFlags & OBJ_EVENT_KEEPS_SPEED_ON_SLOPES) == 0) {
            dx = object->velocityX;
            dy = object->velocityY;
            dz = object->velocityZ;
            speed = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                              Iwram_MulQ16(dz, dz));
            if (speed != 0) {
              reducedSpeed = speed - velocityY;
              if (reducedSpeed < 0)
                reducedSpeed = 0;
              ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
              SET_VELOCITY(object, Iwram_MulQ16(dx, ratio),
                           Iwram_MulQ16(dy, ratio), Iwram_MulQ16(dz, ratio));
            }
          }
          object->terrainHeight = ground;
        }

        // Gravity, and a bounce that loses energy on landing.
        if ((object->motionFlags & OBJ_EVENT_FALLS) != 0) {
          ground = object->terrainHeight;
          if (y > ground) {
            object->velocityY -= object->gravity;
          } else if (object->velocityY < 0) {
            y = ground;
            object->velocityY =
                -Iwram_MulQ16(object->velocityY, object->vertical.bounce);
            // A rebound no faster than one frame of gravity ends the bouncing.
            if ((object->velocityY < 0 ? -object->velocityY
                                       : object->velocityY) <= object->gravity)
              object->velocityY = 0;
          }
        }

        // Float: bob up and down on a triangle wave (OBJ_EVENT_FLOATS_HIGH
        // makes it bigger). The phase runs over 64 frames, each table entry
        // held for two. The first 32 frames' velocities sum to 128 * gravity /
        // 64, so the object rises and falls by about 2 * gravity pixels, or 8 *
        // gravity with OBJ_EVENT_FLOATS_HIGH (2 and 8 pixels at the default
        // gravity of 1.0).
        if ((object->motionFlags & OBJ_EVENT_FLOATS) != 0) {
          phase = (object->vertical.phase & 0x3f);
          if ((object->motionFlags & OBJ_EVENT_FLOATS_HIGH) != 0)
            object->velocityY =
                gScriptObject_CycleTable[phase >> 1] * object->gravity / 16;
          else
            object->velocityY =
                gScriptObject_CycleTable[phase >> 1] * object->gravity / 64;
          object->vertical.phase++;
        }
      }
    }

    x += object->velocityX;
    y += object->velocityY;
    z += object->velocityZ;

    // An object with OBJ_EVENT_BLOCKED_BY_SOLID does not move into an
    // OBJ_EVENT_SOLID one; count the refusals. A refused object keeps last
    // frame's position and skips arrival and turning; SC_SYNC stops waiting for
    // it after 60.
    if ((object->collisionFlags & OBJ_EVENT_BLOCKED_BY_SOLID) != 0) {
      SET_VECTOR(pos, x, y, z);
      if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object, pos) !=
          0) {
        object->refused++;
        continue;
      }
      object->refused = 0;
    }

    // Arrived: the watched axis reached the target, or crossed it.
    switch (object->arrivalAxis) {
    case ARRIVAL_AXIS_X:
      if (x == object->targetX ||
          ((object->x - object->targetX) ^ (x - object->targetX)) < 0)
        arrivedAtTarget = 1;
      break;
    case ARRIVAL_AXIS_Y:
      if (y == object->targetY ||
          ((object->y - object->targetY) ^ (y - object->targetY)) < 0)
        arrivedAtTarget = 1;
      break;
    case ARRIVAL_AXIS_Z:
      if (z == object->targetZ ||
          ((object->z - object->targetZ) ^ (z - object->targetZ)) < 0)
        arrivedAtTarget = 1;
      break;
    default:
      break;
    }

    if (arrivedAtTarget != 0) {
      if (object->snapToTarget != 0) {
        x = object->targetX;
        object->velocityX = 0;
        z = object->targetZ;
        object->velocityZ = 0;
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

    SET_POSITION(object, x, y, z);

    // Turn to face the direction of travel, at most a sixteenth of a turn
    // (0x1000) a frame.
    if ((object->steeringFlags & 1) != 0) {
      x = object->velocityX;
      z = object->velocityZ;
      if (x != 0 || z != 0) {
        s32 maximumFacingStep;

        // As a signed halfword the difference is the shorter way round.
        facingStep = (s16)(ArcTan2(z, x) - object->facing);
        maximumFacingStep = 0x1000;
        if (facingStep > maximumFacingStep)
          facingStep = maximumFacingStep;
        maximumFacingStep = -0x1000;
        if (facingStep < maximumFacingStep)
          facingStep = maximumFacingStep;
        object->facing += facingStep;
      }
    }
  }
}

/// Places the object at (x, y, z), in 16.16 pixels, clears its movement target
/// and stops it. Reached through SC_POSITION and FieldObject_Create, and as
/// Object_SetPositionAndResetMotionFar from the Psynergy effects and the
/// field map's object events.
void Object_SetPositionAndResetMotion(struct ObjectRuntime *object, s32 x,
                                      s32 y, s32 z) {
  object->z = z;
  object->targetX = TARGET_UNSET;
  object->targetY = TARGET_UNSET;
  object->targetZ = TARGET_UNSET;
  object->x = x;
  object->y = y;
  SET_VELOCITY(object, 0, 0, 0);
}

/// Sends an object towards (x, y, z), in 16.16 pixels. Within a pixel it
/// arrives at once. Unless it snaps to its target, the target is pulled in
/// by half its braking distance (speed limit squared over acceleration), or
/// to halfway when that is further, as the object coasts on. Picks the
/// axis to watch for arrival: the one with the furthest to go (y only for a
/// free-flying object).
///
/// Reached through SC_MOVE and the other move commands, the player control,
/// and as Object_SetMoveTargetFar from the battle's motion, the battle
/// effects and the Psynergy effects. Sets the target and arrivalAxis.
void Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z) {
  s32 dx;
  s32 distance;
  s32 dz;
  s32 squaredZ;
  u8 *arrivalAxis;

  dx = (x - object->x) / 0x10000;
  distance = (y - object->y) / 0x10000;
  dz = (z - object->z) / 0x10000;
  squaredZ = dz * dz;
  distance = Iwram_Sqrt(dx * dx + distance * distance + squaredZ) << 16;
  // Under 16 whole pixels, measure again in 16.16 for sub-pixel accuracy.
  if (distance < 0x100000) {
    dx = x - object->x;
    distance = y - object->y;
    dz = z - object->z;
    distance =
        FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(distance, distance) +
                  Iwram_MulQ16(dz, dz));
  }
  if (distance < 0x10000) {
    SET_POSITION(object, x, y, z);
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    return;
  }
  if (object->snapToTarget == 0) {
    s32 brakingDistance;

    // speedLimit squared over acceleration, in 16.16 pixels: twice the
    // distance an object at its limit coasts while it brakes to a stop, so
    // the shortened target leaves it room to coast onto the point asked for.
    brakingDistance =
        Iwram_RatioMulQ14(object->acceleration,
                          Iwram_MulQ16(object->speedLimit, object->speedLimit));
    if (distance > brakingDistance)
      brakingDistance = distance - brakingDistance / 2;
    else
      brakingDistance = distance / 2;
    // Scale the offset to the target by the shortened distance's share of
    // the whole.
    distance = Iwram_RatioMulQ14(distance, brakingDistance);
    x = object->x + Iwram_MulQ16(x - object->x, distance);
    y = object->y + Iwram_MulQ16(y - object->y, distance);
    z = object->z + Iwram_MulQ16(z - object->z, distance);
  }
  object->targetX = x;
  object->targetY = y;
  object->targetZ = z;
  // Watch the axis with the furthest to go: a crossing on an axis with
  // little to go can come long before the target is reached. y only counts
  // in free flight (no motion flags); otherwise the ground sets the height.
  dx = x - object->x;
  distance = y - object->y;
  dz = z - object->z;
  arrivalAxis = &object->arrivalAxis;
  *arrivalAxis = ARRIVAL_AXIS_X;
  if ((dx < 0 ? -dx : dx) < (dz < 0 ? -dz : dz)) {
    *arrivalAxis = ARRIVAL_AXIS_Z;
    dx = dz;
  }
  if (object->flags == 0) {
    if (dx < 0)
      dx = -dx;
    if (dx < (distance < 0 ? -distance : distance))
      *arrivalAxis = ARRIVAL_AXIS_Y;
  }
}

/// Unused: copies Object_UpdateAll (ARM code) to the heap, runs it once and
/// frees it.
/// Nothing calls it.
UNUSED void Object_RunUpdateAllFromHeap(void) {
  void (*updateObjects)(void);

  RamCode_Load(updateObjects, Object_UpdateAll, Object_UpdateAllCodeSize);
  updateObjects();
  MemFree(updateObjects);
}

/// Advances the first 14 objects' motion by one frame, without running their
/// scripts: steering towards a target or braking, gravity and bouncing,
/// arrival, and turning to face the direction of travel.
/// ObjectSystem_Initialize schedules it in place of Object_UpdateAllThumb in
/// MODE_BATTLE.
void Object_UpdateAllMotion(void) {
  struct ScriptMotionObject *object;
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

  // Fourteen objects, as many as the battle has units (UNIT_MAX in
  // battle/battle/objects.c).
  object = (struct ScriptMotionObject *)gObjectSlots;
  for (objectsRemaining = 13; objectsRemaining >= 0;
       objectsRemaining--, object++) {
    if (object->script == 0)
      continue;
    x = object->x;
    y = object->y;
    z = object->z;
    // UB: a frozen object otherwise reads an uninitialized arrival flag, or
    // inherits the preceding object's flag. Initializing it here changes the
    // EN load/store schedule while keeping the 788-byte function extent.
#ifdef UBFIX
    arrivedAtTarget = 0;
#endif
    if (object->frozen == 0) {
#ifndef UBFIX
      arrivedAtTarget = 0;
#endif
      // Steer in the XZ plane as Object_UpdateAllThumb does on the ground,
      // or brake; this loop has no free flight, terrain or floating.
      if (object->targetX != TARGET_UNSET) {
        dx = (object->targetX - x) / 65536;
        dz = (object->targetZ - z) / 65536;
        distance = Iwram_Sqrt(dx * dx + dz * dz) << 16;
        // Under 256 pixels, measure again in 16.16: the root of a 16.16
        // square is 8.8, and << 8 makes it 16.16 again.
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
          velocityX = object->velocityX + Iwram_MulQ16(dx, ratio);
          object->velocityX = velocityX;
          dx = object->velocityZ + Iwram_MulQ16(dz, ratio);
          object->velocityZ = dx;
          distance = Iwram_Sqrt(Iwram_MulQ16(velocityX, velocityX) +
                                Iwram_MulQ16(dx, dx))
                     << 8;
          if (distance > object->speedLimit) {
            ratio = Iwram_RatioMulQ14(distance, object->speedLimit);
            object->velocityX = Iwram_MulQ16(velocityX, ratio);
            object->velocityZ = Iwram_MulQ16(dx, ratio);
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
      // Gravity, and a bounce that loses energy on landing.
      if (object->motionFlags & OBJ_EVENT_FALLS) {
        if (y > object->terrainHeight) {
          object->velocityY -= object->gravity;
        } else if (object->velocityY < 0) {
          y = object->terrainHeight;
          object->velocityY =
              -Iwram_MulQ16(object->velocityY, object->vertical.bounce);
          if ((object->velocityY < 0 ? -object->velocityY
                                     : object->velocityY) <= object->gravity)
            object->velocityY = 0;
        }
      }
    }
    x += object->velocityX;
    y += object->velocityY;
    z += object->velocityZ;
    // Arrived: the watched axis reached the target, or crossed it.
    if (object->arrivalAxis != 0)
      switch (object->arrivalAxis) {
      case ARRIVAL_AXIS_X:
        if (x == object->targetX ||
            ((object->x - object->targetX) ^ (x - object->targetX)) < 0)
          arrivedAtTarget = 1;
        break;
      case ARRIVAL_AXIS_Y:
        if (y == object->targetY ||
            ((object->y - object->targetY) ^ (y - object->targetY)) < 0)
          arrivedAtTarget = 1;
        break;
      case ARRIVAL_AXIS_Z:
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
    SET_POSITION(object, x, y, z);
    // Turn to face the direction of travel, at most a sixteenth of a turn a
    // frame, the shorter way round.
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
  }
}
