// Near miss, differ 81 of 324: script command 0x1d, the ladder sibling of
// Func_080292fc: moves an object to the nearest ladder point within 32 pixels
// of the direction pad's step, turning it toward the pad by at most 0x1000
// when refused or arrived. Remaining: the angle table's address loads
// before the held keys natively (an empty asm on the table gets it after
// the input's address, differ 79); register choice in the best point's
// position test (sl as base, the native's r1); sched2 order around the
// copies, as in Func_08025698. The spilled locals are declared in reverse
// stack order and the ladder loop counts up, as in Func_080292fc.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "event_work.h"
#include "object_runtime.h"
#include "iwram_call.h"

struct FieldPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct LadderPair {
  u8 unk00[8];
  s32 speed;
  u8 unk0c[8];
  s32 moving;
  u8 unk18[4];
};

struct LadderPoint {
  u8 unk00[4];
  s32 x;
  s32 y;
  s32 z;
  s8 active;
  s8 pair;
  u8 unk12[14];
};

struct LadderWork {
  u8 unk000[12];
  struct LadderPair pairs[8];
  struct LadderPoint points[128];
};

struct SpriteMode {
  u8 unk00[23];
  u8 mode;
};

extern const u16 gData_0802ec5c[16];

#define LADDER_WORK (*(struct LadderWork **)&Ram_HeapSlots->unk84[7])

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
void ObjectDispatch_SelectAnimation(struct ObjectRuntime *object,
                                            s32 argument);
void ObjectDispatch_SetAnimationSpeed(struct ObjectRuntime *object,
                                         s32 value);
s32 Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z);

static inline void TurnToward(struct ObjectRuntime *object, u16 angle) {
  s16 delta = angle - object->angle;

  if (delta > 0x1000)
    delta = 0x1000;
  if (delta < -0x1000)
    delta = -0x1000;
  object->angle += delta;
}

s32 Func_08029570(struct ObjectRuntime *object) {
  struct FieldPosition position;
  s32 collisionFlags;
  struct SpriteMode *sprite;
  s32 argument;
  struct LadderWork *ladder;
  struct LadderPoint *best;
  struct LadderPoint *point;
  struct LadderPair *pair;
  s32 bestDistance;
  u16 angle;
  s32 count;
  struct EventWork *work;

  sprite = object->animation;
  argument = 1;
  collisionFlags = 0;
  object->speedLimit = 0x8000;
  object->acceleration = 0x8000;
  object->motionFlags = collisionFlags;
  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  if (angle == 0xffff) {
    collisionFlags = 4;
    best = (struct LadderPoint *)object->linkedObject;
    position.x = object->x;
    position.y = best->y;
    position.z = object->z;
  } else {
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x80000, angle, &position);
    ladder = LADDER_WORK;
    point = ladder->points;
    best = NULL;
    bestDistance = 0x10000000;
    if (ladder == NULL) {
      collisionFlags = 1;
    } else {
      for (count = 0; count < 128; count++, point++) {
        if (point->active == 1) {
          s32 dx = (position.x - point->x) >> 8;
          s32 dy = (position.y - point->y) >> 8;
          s32 dz = (position.z - point->z) >> 8;
          s32 distance = Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                         Iwram_MulQ16(dz, dz);

          if (distance <= 0x400 && bestDistance > distance) {
            bestDistance = distance;
            best = point;
          }
        }
      }
      if (best == NULL) {
        collisionFlags |= 1;
      } else if (best->x == object->x && best->y == object->y &&
                 best->z == object->z) {
        collisionFlags = 1;
      } else {
        if (best->pair != 0) {
          pair = &ladder->pairs[best->pair - 1];
          pair->moving = 1;
          pair->speed += 0x8000;
          if (pair->speed > 0x20000)
            pair->speed = 0x20000;
          object->linkedObject = (struct ObjectRuntime *)best;
        }
        position.x = best->x;
        position.y = best->y;
        position.z = best->z;
      }
    }
  }
  work = Ram_HeapSlots->eventWork;
  if (work != NULL) {
    if (collisionFlags & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  if (collisionFlags == 0)
    argument = 2;
  ObjectDispatch_SelectAnimation(object, argument);
  ObjectDispatch_SetAnimationSpeed(object, 8);
  sprite->mode = 4;
  if (collisionFlags != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
    if (collisionFlags & 3)
      TurnToward(object, angle);
    object->y = position.y;
  } else {
    object->y = position.y;
    Object_SetMoveTarget(object, position.x, position.y, position.z);
    if (object->x == position.x && object->z == position.z)
      TurnToward(object, angle);
  }
  object->step++;
  return 1;
}
