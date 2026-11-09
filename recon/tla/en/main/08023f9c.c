// Draft, differ 4 of 875 (tla-en, 2026-11-08)
// Devices pass (2026-11-09), none below 4: after the first product, an asm
// on ratio or on the product, clobbering r6 or reading the stored velocityX
// (6 each: the order stays and the asm's 4 bytes move the next pool).
// All six editions differ by the same 4. Left: one swap in the slope
// slowdown; every other instruction and register agrees. After the first
// Iwram_MulQ16 product the native runs `adds r1, r5` (the next argument),
// `str r0, [r7, #36]` (velocityX), then `ldr r6, =Iwram_MulQ16` (reload's
// rematerialised pointer for the second and third products); the draft puts
// the ldr first.
// Why no source form closes it (sched2 -fsched-verbose dump, 2026-11-08):
// that reload is `(set r6 (const_int))`, and sched2 gives a constant load no
// dependence at all (r6 is call-saved). After the call it ties the store and
// the copy on priority (69 each) and wins rank_for_schedule's class test:
// independent beats the copy's anti-dependence on the call and the store's
// data dependence on it. The native order is exactly what sched2 gives when
// that load depends on the call, the copy or the store, as if the compiler
// read a pool constant as memory after the call's memory flush. Nothing in C
// gives a constant load a dependence: a local pointer pinned to r6 and set
// after the store is still hoisted (20); storing through a velocity pointer
// regroups the block (163); a full barrier after the store holds the copy
// below the store (4) and one anywhere else orders the copy after the store;
// a global register variable would add the dependence but takes r6 from the
// whole function. This is a compiler question (K3), not a source one.
// Tried before without gain: a block-local ratio or function pointer, a
// once-only block, nested tests, reversed copies, the products through dx,
// dy or dz (47 to 49), a once-only block after or around the first store (80,
// 82), the ratio in speed or reducedSpeed (127, 113), the tests as == 0 (4),
// and 32 permuter minutes in all (best 49 when randomising the slowdown).
// 2026-11-07: two of 2026-11-01's three swaps are gone with no source change.
// Written from The Broken Seal's Actor_UpdateAllThumb
// (games/tbs/src/field/field/actor_motion.c), which runs each object's hook
// and script and moves it toward its target each frame (Func_080230e0 adds it
// at 0x48b). This game's changes: a paused object records no motion and is
// skipped; ground following lets an object (priorityFlags bit 6) stand on
// another (bit 7) 16 pixels below in the cell it moves into, remembering it at
// 0x7c and keeping its speed there; a fall lands at the ground height less the
// rebound; each object records the distance it moved this frame at 0x70; the
// arrival axes are numbered 16 to 18. Also tried without gain: the order of
// the velocity sums and of standing and standingOn, operand order, a != 0
// test, pos's order, a velocityX temporary in the slowdown.
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"

/// The words of an object's script. As in The Broken Seal's ActorScriptWords,
/// a flat word pointer changes one load's base and index order.
struct MotionScript {
  s32 word[1];
};

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
extern const s32 gData_0802ebc8[];

s32 FixedSqrt(s32 value);
s32 ScriptObject_CheckOverlap(struct MotionObject *object, s32 *values);
s32 Func_0802d45c(s32 terrainId, s32 x, s32 z);
s16 ArcTan2(s32 y, s32 x);

#define TARGET_UNSET ((s32)0x80000000)

/// A whole-pixel distance squared.
static __inline__ s32 Object_Square(s32 value) { return value * value; }

void Func_08023f9c(void) {
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
            ground = Func_0802d45c(object->terrainId, velocityX, velocityZ);
          velocityY = ground - object->groundHeight;
          if (ground - y > -0x40000)
            y += velocityY;
          if (velocityY < 0)
            velocityY = -velocityY;
          maximumChange = object->acceleration / 2;
          if (velocityY > maximumChange)
            velocityY = maximumChange;
          velocityY *= 3;
          if (velocityY != 0 && !(object->motionFlags & 16) && !standing) {
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
              object->velocityX = Iwram_MulQ16(dx, ratio);
              object->velocityY = Iwram_MulQ16(dy, ratio);
              object->velocityZ = Iwram_MulQ16(dz, ratio);
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
        if (object->motionFlags & 4) {
          u32 phase = object->bounce & 63;

          if (object->motionFlags & 8)
            object->velocityY =
                gData_0802ebc8[phase >> 1] * object->gravity / 16;
          else
            object->velocityY =
                gData_0802ebc8[phase >> 1] * object->gravity / 64;
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
      if (ScriptObject_CheckOverlap(object, pos) != 0) {
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

        maximumFacingStep = 0x1000;
        if (facingStep > maximumFacingStep)
          facingStep = maximumFacingStep;
        maximumFacingStep = -0x1000;
        if (facingStep < maximumFacingStep)
          facingStep = maximumFacingStep;
        object->angle += facingStep;
      }
    }
  }
}
