// Draft, differ 758 of 875: The Broken Seal's Object_UpdateAllThumb
// (games/tbs/src/field/field/object_motion.c), which runs each object's hook
// and script and moves it toward its target each frame (Func_080230e0 adds
// it at 0x48b). This game's changes: a paused object records no motion and
// is skipped; ground following lets an object (priorityFlags bit 6) stand on
// another (bit 7) 16 pixels below in the cell it moves into, remembering it
// at byte 0x7c and keeping its speed there; a fall lands at the ground
// height less the rebound rather than on the ground; each object records
// the distance it moved this frame at 0x70; the arrival axes are numbered
// 16 to 18. Left: the native keeps a second copy of the motion flags'
// address and the arrival flag in lower stack slots (x, y and z at 24, 20
// and 16), and register allocation differs from the free flight on.
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"

/// An object as the motion update reads it (struct ObjectRuntime's layout).
struct MotionObject {
  s32 *script;
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
extern const s32 gData_0802ebc8[];

s32 FixedSqrt(s32 value);
s32 ScriptObject_CheckOverlap(struct MotionObject *object, s32 *values);
s32 Func_0802d45c(s32 terrainId, s32 x, s32 z);
s16 ArcTan2(s32 y, s32 x);

#define TARGET_UNSET ((s32)0x80000000)

void Func_08023f9c(void) {
  struct MotionObject *object;
  u8 *motionFlags;
  s32 objectsRemaining;
  s32 arrivedAtTarget;
  s32 x;
  s32 y;
  s32 z;
  s32 ground;
  s32 pos[3];
  s32 dx;
  s32 dy;
  s32 dz;
  s32 distance;
  s32 speed;
  s32 ratio;
  s32 change;
  s32 standing;

  object = Ram_HeapSlots->scriptObjects;
  for (objectsRemaining = 63, motionFlags = &object->motionFlags;
       objectsRemaining >= 0;
       objectsRemaining--, motionFlags += sizeof(*object), object++) {
    s32 *script;

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
    if (motionFlags[6] != 0) {
      object->moved[0] = arrivedAtTarget;
      object->moved[1] = arrivedAtTarget;
      object->moved[2] = arrivedAtTarget;
      continue;
    }

    if (object->wait != 0) {
      object->wait--;
    } else {
      for (;;) {
        u32 command = *(script + object->step);

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
    if (motionFlags[12] != 0)
      goto move;

    if (*motionFlags == 0) {
      if (object->targetX != TARGET_UNSET) {
        dx = (object->targetX - x) / 65536;
        dy = (object->targetY - y) / 65536;
        dz = (object->targetZ - z) / 65536;
        distance = Iwram_Sqrt(dx * dx + dy * dy + dz * dz);
        if (distance == 0) {
          x = object->targetX;
          y = object->targetY;
          z = object->targetZ;
        } else {
          s32 velocityX;
          s32 velocityY;
          s32 velocityZ;

          ratio = Iwram_RatioMulQ14(distance << 16, object->acceleration);
          velocityX = object->velocityX + dx * ratio;
          velocityY = object->velocityY + dy * ratio;
          velocityZ = object->velocityZ + dz * ratio;
          object->velocityZ = velocityZ;
          object->velocityY = velocityY;
          object->velocityX = velocityX;
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
          s32 reducedSpeed = speed - object->acceleration;

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
      goto move;
    }

    // On the ground: steer in the XZ plane the same way.
    if (object->targetX != TARGET_UNSET) {
      dx = (object->targetX - x) / 65536;
      dz = (object->targetZ - z) / 65536;
      distance = Iwram_Sqrt(dx * dx + dz * dz) << 16;
      if (distance <= 0x00ffffff) {
        dx = object->targetX - x;
        dz = object->targetZ - z;
        distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
      }
      if (distance == 0) {
        x = object->targetX;
        z = object->targetZ;
      } else {
        s32 velocityX;
        s32 velocityZ;

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
        s32 reducedSpeed = speed - object->acceleration;

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
    if (*motionFlags & 1) {
      s32 nextX = x + object->velocityX;
      s32 nextZ = z + object->velocityZ;

      object->standingOn = NULL;
      standing = 0;
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
          if (((nextX - other->x + 0x80000) >> 20) != 0)
            continue;
          if (((nextZ - other->z + 0x80000) >> 20) != 0)
            continue;
          dy = y - other->y;
          if ((dy - 0x100000 < 0 ? -(dy - 0x100000) : dy - 0x100000) > 0x7ffff)
            continue;
          ground = other->y + 0x100000;
          standing = 1;
          object->standingOn = other;
          break;
        }
      }
      if (!standing)
        ground = Func_0802d45c(object->terrainId, nextX, nextZ);
      change = ground - object->groundHeight;
      if (ground - y > -0x40000)
        y += change;
      if (change < 0)
        change = -change;
      if (change > object->acceleration / 2)
        change = object->acceleration / 2;
      change *= 3;
      if (change != 0 && !(*motionFlags & 16) && !standing) {
        s32 velocityX = object->velocityX;
        s32 velocityY = object->velocityY;
        s32 velocityZ = object->velocityZ;

        speed = FixedSqrt(Iwram_MulQ16(velocityX, velocityX) +
                          Iwram_MulQ16(velocityY, velocityY) +
                          Iwram_MulQ16(velocityZ, velocityZ));
        if (speed != 0) {
          s32 reducedSpeed = speed - change;

          if (reducedSpeed < 0)
            reducedSpeed = 0;
          ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
          object->velocityX = Iwram_MulQ16(velocityX, ratio);
          object->velocityY = Iwram_MulQ16(velocityY, ratio);
          object->velocityZ = Iwram_MulQ16(velocityZ, ratio);
        }
      }
      object->groundHeight = ground;
    }

    // Gravity, and a bounce that loses energy on landing.
    if (*motionFlags & 2) {
      ground = object->groundHeight;
      if (y > ground)
        object->velocityY -= object->gravity;
      if (y + object->velocityY <= ground) {
        if (object->velocityY < 0) {
          s32 rebound = -Iwram_MulQ16(object->velocityY, object->bounce);

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
    if (*motionFlags & 4) {
      u32 phase = object->bounce & 63;

      if (*motionFlags & 8)
        object->velocityY = gData_0802ebc8[phase >> 1] * object->gravity / 16;
      else
        object->velocityY = gData_0802ebc8[phase >> 1] * object->gravity / 64;
      object->bounce++;
    }

  move:
    x += object->velocityX;
    y += object->velocityY;
    z += object->velocityZ;
    if (motionFlags[4] & 0x80) {
      pos[0] = x;
      pos[1] = y;
      pos[2] = z;
      if (ScriptObject_CheckOverlap(object, pos) != 0) {
        motionFlags[11]++;
        continue;
      }
      motionFlags[11] = 0;
    }

    switch (motionFlags[1]) {
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
      if (motionFlags[3] != 0) {
        x = object->targetX;
        object->velocityX = 0;
        z = object->targetZ;
        object->velocityZ = 0;
        if (*motionFlags == 0) {
          y = object->targetY;
          object->velocityY = 0;
        }
      }
      object->targetX = TARGET_UNSET;
      object->targetY = TARGET_UNSET;
      object->targetZ = TARGET_UNSET;
      motionFlags[1] = 0;
    }

    object->moved[0] = x - object->x;
    object->moved[1] = y - object->y;
    object->moved[2] = z - object->z;
    object->x = x;
    object->y = y;
    object->z = z;

    // Turn to face the direction of travel, at most a sixteenth of a turn a
    // frame.
    if (motionFlags[5] & 1) {
      x = object->velocityX;
      z = object->velocityZ;
      if (x != 0 || z != 0) {
        s32 facingStep = (s16)(ArcTan2(z, x) - object->angle);

        if (facingStep > 0x1000)
          facingStep = 0x1000;
        if (facingStep < -0x1000)
          facingStep = -0x1000;
        object->angle += facingStep;
      }
    }
  }
}
