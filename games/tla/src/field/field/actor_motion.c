/// @file
/// Actor motion, each frame.
// Part of field.c.

#include "../field_internal.h"
#include "fixed_math.h"
#include "module/field/resource_slot.h"
#include "module/tla.h"

s32 Actor_IsTargetUnset(struct Actor *object) {
  s32 first;
  s32 second;

  if (object->motionFlags == 0) {
    second = object->targetX;
    if (second != (s32)0x80000000)
      return 0;
    first = object->targetY;
  } else {
    first = object->targetX;
    second = (s32)0x80000000;
  }
  if (first != second || object->targetZ != first)
    return 0;
  return 1;
}

/// Actor_IsTargetUnset, for the scenes: 1 once the actor has reached its
/// destination. Airs Rock waits on it while the lift carries the leader up.
s32 Actor_HasArrived(struct Actor *object) {
  return Actor_IsTargetUnset(object);
}

/// An object as the motion update reads it (struct Actor's layout).
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
  s32 unk4c;
  void *animation;
  u8 animationKind;
  u8 motionFlags;
  u8 arrivalAxis;
  u8 unk57;
  u8 snapToTarget;
  u8 collisionFlags;
  u8 actionFlags;
  u8 paused;
  u8 unk5c[2];
  s16 wait;
  u8 refused;
  u8 frozen;
  u8 unk62[0x0a];
  void (*update)(struct MotionObject *object);
  s32 moved[3];
  struct MotionObject *standingOn;
};

typedef s32 (*MotionCommand)(struct MotionObject *object);

extern const MotionCommand gData_0802f204[];
extern const s32 gActorScript_CycleTable[];

#define TARGET_UNSET ((s32)0x80000000)

/// A whole-pixel distance squared.
static __inline__ s32 Object_Square(s32 value) { return value * value; }

void Actor_UpdateAllThumb(void) {
  struct MotionObject *object;
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
  s32 reducedSpeed;
  s32 ratio;
  s32 maximumChange;
  s32 standing;

  object = Ram_HeapSlots->scriptObjects;
  for (objectsRemaining = 63; objectsRemaining >= 0;
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

        if (command <= 63) {
          if (gData_0802f204[command](object) == 0)
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
        if (object->targetX != TARGET_UNSET) {
          dx = (object->targetX - x) / 65536;
          dy = (object->targetY - y) / 65536;
          dz = (object->targetZ - z) / 65536;
          distance = Iwram_Sqrt(Object_Square(dx) + Object_Square(dy) +
                                Object_Square(dz));
          if (distance == 0) {
            x = object->targetX;
            y = object->targetY;
            z = object->targetZ;
          } else {
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
            if (speed > object->speedLimit) {
              ratio = Iwram_RatioMulQ14(speed, object->speedLimit);
              object->velocityX = Iwram_MulQ16(velocityX, ratio);
              object->velocityY = Iwram_MulQ16(velocityY, ratio);
              object->velocityZ = Iwram_MulQ16(velocityZ, ratio);
            }
          }
        } else {
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
            object->velocityX = Iwram_MulQ16(dx, ratio);
            object->velocityY = Iwram_MulQ16(dy, ratio);
            object->velocityZ = Iwram_MulQ16(dz, ratio);
          } else {
            object->velocityX = speed;
            object->velocityY = speed;
            object->velocityZ = speed;
          }
        }
      } else {
        // On the ground: steer in the XZ plane the same way.
        if (object->targetX != TARGET_UNSET) {
          dx = (object->targetX - x) / 65536;
          dz = (object->targetZ - z) / 65536;
          distance = Iwram_Sqrt(Object_Square(dx) + Object_Square(dz)) << 16;
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
            object->velocityX = speed;
            object->velocityZ = speed;
          }
        }

        // Follow the ground, or the top of an object 16 pixels below.
        if (object->motionFlags & 1) {
          velocityX = x + object->velocityX;
          velocityZ = z + object->velocityZ;
          standing = 0;
          object->standingOn = NULL;
          if (object->priorityFlags & 0x40) {
            struct MotionObject *other = Ram_HeapSlots->scriptObjects;
            s32 i;

            for (i = 0; i < 64; i++, other++) {
              s32 dy;

              if (other->script == NULL)
                continue;
              if (!(other->priorityFlags & 0x80))
                continue;
              if (other == object)
                continue;
              if (((velocityX - other->x + 0x80000) >> 20) != 0)
                continue;
              if (((velocityZ - other->z + 0x80000) >> 20) != 0)
                continue;
              dy = y - other->y;
              if ((dy - 0x100000 < 0 ? -(dy - 0x100000) : dy - 0x100000) >
                  0x7ffff)
                continue;
              standing = 1;
              ground = other->y + 0x100000;
              object->standingOn = other;
              break;
            }
          }
          if (!standing)
            ground = MapGrid_GetTerrainHeight(object->terrainId, velocityX,
                                              velocityZ);
          velocityY = ground - object->groundHeight;
          if (ground - y > -0x40000)
            y += velocityY;
          if (velocityY < 0)
            velocityY = -velocityY;
          maximumChange = object->acceleration / 2;
          if (velocityY > maximumChange)
            velocityY = maximumChange;
          velocityY *= 3;
          // This slowdown and the landing bounce below call IwramMulQ16 by its
          // linked name, so each call reads the routine's address from the
          // literal pool after the call before it.
          if (velocityY != 0 && !(object->motionFlags & 16) && !standing) {
            dx = object->velocityX;
            dy = object->velocityY;
            dz = object->velocityZ;
            speed = FixedSqrt(IwramMulQ16(dx, dx) + IwramMulQ16(dy, dy) +
                              IwramMulQ16(dz, dz));
            if (speed != 0) {
              reducedSpeed = speed - velocityY;
              if (reducedSpeed < 0)
                reducedSpeed = 0;
              ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
              object->velocityX = IwramMulQ16(dx, ratio);
              object->velocityY = IwramMulQ16(dy, ratio);
              object->velocityZ = IwramMulQ16(dz, ratio);
            }
          }
          object->groundHeight = ground;
        }

        // Gravity, and a bounce that loses energy on landing.
        if (object->motionFlags & 2) {
          ground = object->groundHeight;
          if (y > ground)
            object->velocityY -= object->gravity;
          if (y + object->velocityY <= ground) {
            if (object->velocityY < 0) {
              s32 rebound = -IwramMulQ16(object->velocityY, object->bounce);

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
        if (object->motionFlags & 4) {
          u32 phase = object->bounce & 63;

          if (object->motionFlags & 8)
            object->velocityY =
                gActorScript_CycleTable[phase >> 1] * object->gravity / 16;
          else
            object->velocityY =
                gActorScript_CycleTable[phase >> 1] * object->gravity / 64;
          object->bounce++;
        }
      }
    }

    x += object->velocityX;
    y += object->velocityY;
    z += object->velocityZ;
    if (object->collisionFlags & 0x80) {
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
    case 16:
      if (x == object->targetX ||
          ((object->x - object->targetX) ^ (x - object->targetX)) < 0)
        arrivedAtTarget = 1;
      break;
    case 17:
      if (y == object->targetY ||
          ((object->y - object->targetY) ^ (y - object->targetY)) < 0)
        arrivedAtTarget = 1;
      break;
    case 18:
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
      object->targetX = TARGET_UNSET;
      object->targetY = TARGET_UNSET;
      object->targetZ = TARGET_UNSET;
      object->arrivalAxis = 0;
    }

    object->moved[0] = x - object->x;
    object->moved[1] = y - object->y;
    object->moved[2] = z - object->z;
    object->x = x;
    object->y = y;
    object->z = z;

    // Turn to face the direction of travel, at most a sixteenth of a turn a
    // frame.
    if (object->actionFlags & 1) {
      x = object->velocityX;
      z = object->velocityZ;
      if (x != 0 || z != 0) {
        s32 facingStep;
        s32 maximumFacingStep;

        facingStep = (s16)(ArcTan2(z, x) - object->angle);

        maximumFacingStep = TRIG_TURN / 16;
        if (facingStep > maximumFacingStep)
          facingStep = maximumFacingStep;
        maximumFacingStep = -TRIG_TURN / 16;
        if (facingStep < maximumFacingStep)
          facingStep = maximumFacingStep;
        object->angle += facingStep;
      }
    }
  }
}

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

/// Task: each actor riding another (priority flag 64, its carrier at offset
/// 0x7c) moves with its carrier's last step (the carrier's words at 0x70,
/// 0x74 and 0x78), and one standing on it is set on its top, 16 pixels up.
void Actor_MoveWithCarriers(void) {
  struct Actor *object = Ram_HeapSlots->scriptObjects;
  s32 i;

  for (i = 63; i >= 0; i--, object++) {
    if (object->script != NULL) {
      s32 x = object->x;
      s32 y = object->y;
      s32 z = object->z;

      if (object->priorityFlags & 64) {
        struct Actor *carrier = (struct Actor *)WORD_AT(object, 0x7c);

        if (carrier != NULL) {
          if (WORD_AT(carrier, 0x74) != 0) {
            if (y == object->terrainHeight) {
              object->terrainHeight = carrier->y + 0x100000;
              y = object->terrainHeight;
            } else {
              object->terrainHeight += WORD_AT(carrier, 0x74);
              y += WORD_AT(carrier, 0x74);
            }
          }
          x += WORD_AT(carrier, 0x70);
          z += WORD_AT(carrier, 0x78);
        }
      }
      SET_POSITION(object, x, y, z);
    }
  }
}

// Objects: set position and cancel motion.

/// Sets the position, clears the destination and zeroes the velocity.
/// obj: object.
/// x, y, z: 16.16 fixed-point position.
void Actor_SetPositionAndResetMotion(struct Actor *obj, s32 x, s32 y, s32 z) {
  obj->z = z;
  obj->targetX = 0x80000000; // No destination.
  obj->targetY = 0x80000000;
  obj->targetZ = 0x80000000;
  obj->x = x;
  obj->y = y;
  SET_VELOCITY(obj, 0, 0, 0);
}

void Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z) {
  s32 dx;
  s32 distance;
  s32 dz;
  s32 squaredZ;
  s32 dy;
  u8 *arrivalAxis;

  dx = (x - object->x) / Q16_ONE;
  dy = (y - object->y) / Q16_ONE;
  dz = (z - object->z) / Q16_ONE;
  squaredZ = dz * dz;
  distance = Iwram_Sqrt(dx * dx + dy * dy + squaredZ) << 16;
  if (distance < 0x100000) {
    dx = x - object->x;
    dy = y - object->y;
    dz = z - object->z;
    distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                         Iwram_MulQ16(dz, dz));
  }
  if (distance < 0x10000) {
    object->x = x;
    object->y = y;
    object->z = z;
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    return;
  }
  if (object->snapToTarget == 0) {
    s32 brakingDistance;
    s32 ratio;

    brakingDistance =
        Iwram_RatioMulQ14(object->acceleration,
                          Iwram_MulQ16(object->speedLimit, object->speedLimit));
    if (distance > brakingDistance)
      brakingDistance = distance - brakingDistance / 2;
    else
      brakingDistance = distance / 2;
    ratio = Iwram_RatioMulQ14(distance, brakingDistance);
    x = object->x + Iwram_MulQ16(x - object->x, ratio);
    y = object->y + Iwram_MulQ16(y - object->y, ratio);
    z = object->z + Iwram_MulQ16(z - object->z, ratio);
  }
  object->targetX = x;
  object->targetY = y;
  object->targetZ = z;
  dx = x - object->x;
  dy = y - object->y;
  dz = z - object->z;
  arrivalAxis = &object->unk56;
  *arrivalAxis = 16;
  if ((dx < 0 ? -dx : dx) < (dz < 0 ? -dz : dz)) {
    *arrivalAxis = 18;
    dx = dz;
  }
  if (object->motionFlags == 0) {
    if (dx < 0)
      dx = -dx;
    if (dx < (dy < 0 ? -dy : dy))
      *arrivalAxis = 17;
  }
}

void ObjectList_CollectActive(u8 *object, u8 **out) {
  u8 *end = object + 0x700;

  do {
    if (*(u16 *)(object + 2) != 0)
      *out++ = object;
    object += 128;
  } while (object != end);
  *out = NULL;
}
#undef TARGET_UNSET
#undef WORD_AT
