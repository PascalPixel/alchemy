/// @file
/// Actors' motion each frame: their scripts, steering toward a target, falling,
/// floating and riding a platform, and being sent somewhere.
// Part of field.c.

#include "field_internal.h"
#include "fixed_math.h"
#include "field/resource_slot.h"
#include "macros.h"

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

/// Actor_IsTargetUnset, for the scenes: 1 once the actor has reached its
/// destination. Airs Rock waits on it while the lift carries the leader up.
s32 Actor_HasArrived(struct Actor *actor) { return Actor_IsTargetUnset(actor); }

/// An actor as the motion update reads it (struct Actor's layout).
struct MotionObject {
  struct MotionScript *script;
  s16 step;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  s32 groundHeight;
  s32 scaleX;
  s32 scaleY;
  u16 radius;
  u8 terrainId;
  u8 priorityFlags;
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
  s32 riseBaseY;
  void *animation;
  u8 animationKind;
  u8 motionFlags;
  u8 arrivalAxis;
  u8 unusedAfterArrivalAxis; ///< no code names it
  u8 snapToTarget;
  u8 collisionFlags;
  u8 actionFlags;
  u8 paused;
  u8 unusedAfterPaused[2]; ///< no code names it
  s16 wait;
  u8 refused;
  u8 frozen;
  u8 unusedAfterFrozen[10]; ///< no code names it
  void (*update)(struct MotionObject *object);
  s32 moved[3];
  struct MotionObject *standingOn;
};

typedef s32 (*MotionCommand)(struct MotionObject *object);

extern const MotionCommand gActorScript_CommandTable[];
extern const s32 gActorScript_CycleTable[];

/// The highest script word read as a command, an index of
/// gActorScript_CommandTable; a larger one is stepped over.
#define SCRIPT_COMMAND_MAX 63
/// A floating actor's bob, one cycle of gActorScript_CycleTable over this many
/// frames; its rise and fall each frame is the table's value times its gravity
/// over FLOAT_DIVISOR, or FLOAT_HIGH_DIVISOR with ACTOR_FLOATS_HIGH.
#define FLOAT_CYCLE_FRAMES 64
#define FLOAT_DIVISOR 64
#define FLOAT_HIGH_DIVISOR 16

/// A whole-pixel distance squared.
static __inline__ s32 Object_Square(s32 value) { return value * value; }

/// Advances every actor by one frame: their scripts and their motion, with
/// free flight in 3D, following the ground, floating, and solid actors that
/// refuse to overlap.
void Actor_UpdateAllThumb(void) {
  struct MotionObject *object;
  s32 objectsRemaining;
  s32 x;
  FIXED y;
  s32 z;
  FIXED ground;
  s32 arrivedAtTarget;
  FIXED pos[3];
  s32 dx;
  s32 dy;
  s32 dz;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 distance;
  FIXED speed;
  FIXED reducedSpeed;
  FIXED ratio;
  s32 maximumChange;
  s32 standing;

  object = Ram_HeapSlots->actors;
  for (objectsRemaining = ACTOR_COUNT - 1; objectsRemaining >= 0;
       objectsRemaining--, object++) {
    struct MotionScript *script;

    arrivedAtTarget = 0;
    script = object->script;
    if (script == NULL)
      continue;
    if (object->update != NULL) {
      object->update(object);
      script = object->script;
    }
    if (script == NULL)
      continue;
    if (object->paused != 0) {
      object->moved[0] = arrivedAtTarget;
      object->moved[1] = arrivedAtTarget;
      object->moved[2] = arrivedAtTarget;
      continue;
    }

    if (object->wait != 0) {
      object->wait--;
    } else {
      for (;;) {
        u32 command = script->word[object->step];

        if (command <= SCRIPT_COMMAND_MAX) {
          if (gActorScript_CommandTable[command](object) == 0)
            break;
        } else {
          object->step++;
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
      if (object->motionFlags == 0) {
        if (object->targetX != ACTOR_NO_TARGET) {
          dx = (object->targetX - x) / FIXED_ONE;
          dy = (object->targetY - y) / FIXED_ONE;
          dz = (object->targetZ - z) / FIXED_ONE;
          distance = Iwram_Sqrt(Object_Square(dx) + Object_Square(dy) +
                                Object_Square(dz));
          if (distance == 0) {
            x = object->targetX;
            y = object->targetY;
            z = object->targetZ;
          } else {
            ratio = Iwram_RatioMulQ14(toFIXED(distance), object->acceleration);
            object->velocityX += dx * ratio;
            object->velocityY += dy * ratio;
            object->velocityZ += dz * ratio;
            velocityX = object->velocityX;
            velocityY = object->velocityY;
            velocityZ = object->velocityZ;
            speed = Math_SqrtFX(Iwram_MulFX(velocityX, velocityX) +
                                Iwram_MulFX(velocityY, velocityY) +
                                Iwram_MulFX(velocityZ, velocityZ));
            if (speed > object->speedLimit) {
              ratio = Iwram_RatioMulQ14(speed, object->speedLimit);
              object->velocityX = Iwram_MulFX(velocityX, ratio);
              object->velocityY = Iwram_MulFX(velocityY, ratio);
              object->velocityZ = Iwram_MulFX(velocityZ, ratio);
            }
          }
        } else {
          dx = object->velocityX;
          dy = object->velocityY;
          dz = object->velocityZ;
          speed = Math_SqrtFX(Iwram_MulFX(dx, dx) + Iwram_MulFX(dy, dy) +
                              Iwram_MulFX(dz, dz));
          if (speed != 0) {
            reducedSpeed = speed - object->acceleration;
            if (reducedSpeed < 0)
              reducedSpeed = 0;
            ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
            object->velocityX = Iwram_MulFX(dx, ratio);
            object->velocityY = Iwram_MulFX(dy, ratio);
            object->velocityZ = Iwram_MulFX(dz, ratio);
          } else {
            object->velocityX = speed;
            object->velocityY = speed;
            object->velocityZ = speed;
          }
        }
      } else {
        // On the ground: steer in the XZ plane the same way.
        if (object->targetX != ACTOR_NO_TARGET) {
          dx = (object->targetX - x) / FIXED_ONE;
          dz = (object->targetZ - z) / FIXED_ONE;
          distance = toFIXED(Iwram_Sqrt(Object_Square(dx) + Object_Square(dz)));
          if (distance <= toFIXED(256) - 1) {
            dx = object->targetX - x;
            dz = object->targetZ - z;
            distance = Math_SqrtFX(Iwram_MulFX(dx, dx) + Iwram_MulFX(dz, dz));
          }
          if (distance == 0) {
            x = object->targetX;
            z = object->targetZ;
          } else {
            ratio = Iwram_RatioMulQ14(distance, object->acceleration);
            object->velocityX += Iwram_MulFX(dx, ratio);
            object->velocityZ += Iwram_MulFX(dz, ratio);
            velocityX = object->velocityX;
            velocityZ = object->velocityZ;
            speed = Math_SqrtFX(Iwram_MulFX(velocityX, velocityX) +
                                Iwram_MulFX(velocityZ, velocityZ));
            if (speed > object->speedLimit) {
              ratio = Iwram_RatioMulQ14(speed, object->speedLimit);
              object->velocityX = Iwram_MulFX(velocityX, ratio);
              object->velocityZ = Iwram_MulFX(velocityZ, ratio);
            }
          }
        } else {
          dx = object->velocityX;
          dz = object->velocityZ;
          speed = Math_SqrtFX(Iwram_MulFX(dx, dx) + Iwram_MulFX(dz, dz));
          if (speed != 0) {
            reducedSpeed = speed - object->acceleration;
            if (reducedSpeed < 0)
              reducedSpeed = 0;
            ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
            object->velocityX = Iwram_MulFX(dx, ratio);
            object->velocityZ = Iwram_MulFX(dz, ratio);
          } else {
            object->velocityX = speed;
            object->velocityZ = speed;
          }
        }

        // Follow the ground, or the top of a platform 16 pixels below.
        if (object->motionFlags & ACTOR_FOLLOWS_TERRAIN) {
          velocityX = x + object->velocityX;
          velocityZ = z + object->velocityZ;
          standing = 0;
          object->standingOn = NULL;
          if (object->priorityFlags & ACTOR_PRIORITY_WALKS_ON_PLATFORMS) {
            struct MotionObject *other = Ram_HeapSlots->actors;
            s32 i;

            for (i = 0; i < ACTOR_COUNT; i++, other++) {
              s32 dy;

              if (other->script == NULL)
                continue;
              if (!(other->priorityFlags & ACTOR_PRIORITY_PLATFORM))
                continue;
              if (other == object)
                continue;
              if ((FIELD_CELL(velocityX - other->x + toFIXED(8))) != 0)
                continue;
              if ((FIELD_CELL(velocityZ - other->z + toFIXED(8))) != 0)
                continue;
              dy = y - other->y;
              if ((dy - toFIXED(16) < 0 ? -(dy - toFIXED(16))
                                        : dy - toFIXED(16)) > toFIXED(8) - 1)
                continue;
              standing = 1;
              ground = other->y + toFIXED(16);
              object->standingOn = other;
              break;
            }
          }
          if (!standing)
            ground = MapGrid_GetTerrainHeight(object->terrainId, velocityX,
                                              velocityZ);
          velocityY = ground - object->groundHeight;
          if (ground - y > -toFIXED(4))
            y += velocityY;
          if (velocityY < 0)
            velocityY = -velocityY;
          maximumChange = object->acceleration / 2;
          if (velocityY > maximumChange)
            velocityY = maximumChange;
          velocityY *= 3;
          if (velocityY != 0 &&
              !(object->motionFlags & ACTOR_KEEPS_SPEED_ON_SLOPES) &&
              !standing) {
            dx = object->velocityX;
            dy = object->velocityY;
            dz = object->velocityZ;
            speed = Math_SqrtFX(IwramMulFX(dx, dx) + IwramMulFX(dy, dy) +
                                IwramMulFX(dz, dz));
            if (speed != 0) {
              reducedSpeed = speed - velocityY;
              if (reducedSpeed < 0)
                reducedSpeed = 0;
              ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
              object->velocityX = IwramMulFX(dx, ratio);
              object->velocityY = IwramMulFX(dy, ratio);
              object->velocityZ = IwramMulFX(dz, ratio);
            }
          }
          object->groundHeight = ground;
        }

        // Gravity, and a bounce that loses energy on landing.
        if (object->motionFlags & ACTOR_FALLS) {
          ground = object->groundHeight;
          if (y > ground)
            object->velocityY -= object->gravity;
          if (y + object->velocityY <= ground) {
            if (object->velocityY < 0) {
              FIXED rebound = -IwramMulFX(object->velocityY, object->bounce);

              object->velocityY = rebound;
              y = ground - rebound;
              if ((rebound < 0 ? -rebound : rebound) <= object->gravity) {
                y = ground;
                object->velocityY = 0;
              }
            } else {
              y = ground;
            }
          }
        }

        // Float: bob up and down on a triangle wave.
        if (object->motionFlags & ACTOR_FLOATS) {
          u32 phase = object->bounce & (FLOAT_CYCLE_FRAMES - 1);

          if (object->motionFlags & ACTOR_FLOATS_HIGH)
            object->velocityY = gActorScript_CycleTable[phase >> 1] *
                                object->gravity / FLOAT_HIGH_DIVISOR;
          else
            object->velocityY = gActorScript_CycleTable[phase >> 1] *
                                object->gravity / FLOAT_DIVISOR;
          object->bounce++;
        }
      }
    }

    x += object->velocityX;
    y += object->velocityY;
    z += object->velocityZ;
    if (object->collisionFlags & ACTOR_BLOCKED_BY_SOLID) {
      pos[0] = x;
      pos[1] = y;
      pos[2] = z;
      if (ActorScript_CheckOverlap(object, pos) != 0) {
        object->refused++;
        continue;
      }
      object->refused = 0;
    }

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
      object->targetX = ACTOR_NO_TARGET;
      object->targetY = ACTOR_NO_TARGET;
      object->targetZ = ACTOR_NO_TARGET;
      object->arrivalAxis = ARRIVAL_AXIS_NONE;
    }

    object->moved[0] = x - object->x;
    object->moved[1] = y - object->y;
    object->moved[2] = z - object->z;
    SET_POSITION(object, x, y, z);

    // Turn to face the direction of travel, at most a sixteenth of a turn a
    // frame.
    if (object->actionFlags & ACTOR_FACES_MOTION) {
      x = object->velocityX;
      z = object->velocityZ;
      if (x != 0 || z != 0) {
        s32 facingStep;
        s32 maximumFacingStep;

        facingStep = (s16)(Trig_Atan(z, x) - object->angle);

        maximumFacingStep = FACING_STEP;
        if (facingStep > maximumFacingStep)
          facingStep = maximumFacingStep;
        maximumFacingStep = -FACING_STEP;
        if (facingStep < maximumFacingStep)
          facingStep = maximumFacingStep;
        object->angle += facingStep;
      }
    }
  }
}

/// Each frame: an actor that walks on platforms
/// (ACTOR_PRIORITY_WALKS_ON_PLATFORMS) and rides one, its carrier, moves with
/// the carrier's last step; one standing on it as it rises or sinks is set on
/// its top, 16 pixels up.
void Actor_MoveWithCarriers(void) {
  struct Actor *actor = Ram_HeapSlots->actors;
  s32 i;

  for (i = 0; i < ACTOR_COUNT; i++, actor++) {
    if (actor->script != NULL) {
      FIXED x = actor->x.fixed;
      FIXED y = actor->y.fixed;
      FIXED z = actor->z.fixed;

      if (actor->priorityFlags & ACTOR_PRIORITY_WALKS_ON_PLATFORMS) {
        struct Actor *carrier = actor->carrier;

        if (carrier != NULL) {
          if (carrier->stepY != 0) {
            if (y == actor->groundHeight) {
              actor->groundHeight = carrier->y.fixed + toFIXED(16);
              y = actor->groundHeight;
            } else {
              actor->groundHeight += carrier->stepY;
              y += carrier->stepY;
            }
          }
          x += carrier->stepX;
          z += carrier->stepZ;
        }
      }
      SET_ACTOR_POSITION(actor, x, y, z);
    }
  }
}

// Placing an actor, and sending it somewhere.

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
  FIXED distance;
  s32 dz;
  s32 squaredZ;
  s32 dy;
  u8 *arrivalAxis;

  dx = (x - actor->x.fixed) / FIXED_ONE;
  dy = (y - actor->y.fixed) / FIXED_ONE;
  dz = (z - actor->z.fixed) / FIXED_ONE;
  squaredZ = dz * dz;
  distance = toFIXED(Iwram_Sqrt(dx * dx + dy * dy + squaredZ));
  if (distance < toFIXED(16)) {
    dx = x - actor->x.fixed;
    dy = y - actor->y.fixed;
    dz = z - actor->z.fixed;
    distance = Math_SqrtFX(Iwram_MulFX(dx, dx) + Iwram_MulFX(dy, dy) +
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
    FIXED brakingDistance;
    FIXED ratio;

    brakingDistance = Iwram_RatioMulQ14(
        actor->acceleration, Iwram_MulFX(actor->speedLimit, actor->speedLimit));
    if (distance > brakingDistance)
      brakingDistance = distance - brakingDistance / 2;
    else
      brakingDistance = distance / 2;
    ratio = Iwram_RatioMulQ14(distance, brakingDistance);
    x = actor->x.fixed + Iwram_MulFX(x - actor->x.fixed, ratio);
    y = actor->y.fixed + Iwram_MulFX(y - actor->y.fixed, ratio);
    z = actor->z.fixed + Iwram_MulFX(z - actor->z.fixed, ratio);
  }
  actor->targetX = x;
  actor->targetY = y;
  actor->targetZ = z;
  dx = x - actor->x.fixed;
  dy = y - actor->y.fixed;
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
    if (dx < (dy < 0 ? -dy : dy))
      *arrivalAxis = ARRIVAL_AXIS_Y;
  }
}

/// Lists those of the first RUNNING_LIST_SLOTS actors whose script is running,
/// in slot order, ended by NULL; out holds one more.
void Actor_ListRunning(struct Actor *actor, struct Actor **out) {
  struct Actor *end = actor + RUNNING_LIST_SLOTS;

  do {
    // A running script's address is above 0xffff, so its upper half alone
    // tells a running actor from a free one.
    if (((u16 *)&actor->script)[1] != 0)
      *out++ = actor;
    actor++;
  } while (actor != end);
  *out = NULL;
}
#undef SCRIPT_COMMAND_MAX
#undef FLOAT_CYCLE_FRAMES
#undef FLOAT_DIVISOR
#undef FLOAT_HIGH_DIVISOR
