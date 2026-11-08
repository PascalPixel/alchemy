#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "event_work.h"
#include "actor.h"
#include "iwram_call.h"
#include "worka0.h"

struct FieldPosition {
  s32 x;
  s32 y;
  s32 z;
};

extern const u16 gData_0802ec5c[16];

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
void Actor_SelectAnimation(struct Actor *object, s32 argument);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
s32 Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);

static inline void TurnToward(struct Actor *object, u16 angle) {
  s16 delta = angle - object->angle;

  if (delta > 0x1000)
    delta = 0x1000;
  if (delta < -0x1000)
    delta = -0x1000;
  object->angle += delta;
}

s32 Func_08029570(struct Actor *object) {
  struct FieldPosition position;
  s32 collisionFlags;
  struct ObjectAnimationState *sprite;
  s32 argument;
  struct WorkA0 *workA0;
  struct WorkA0Node *best;
  struct WorkA0Node *point;
  struct WorkA0Slot *slot;
  s32 bestDistance;
  u16 angle;
  s32 count;
  struct EventWork *work;

  sprite = object->animation;
  argument = 1;
  collisionFlags = 0;
  object->speedLimit = 0x8000;
  object->acceleration = 0x8000;
  object->motionFlags = 0;
  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  if (angle == 0xffff) {
    collisionFlags = 4;
    best = (struct WorkA0Node *)object->watchedObject;
    position.x = object->x;
    position.y = best->y;
    position.z = object->z;
  } else {
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x80000, angle, &position);
    // Read through the slot's address: as Ram_HeapSlots->unk84[7], 44 differ.
    workA0 = *(struct WorkA0 **)&Ram_HeapSlots->unk84[7];
    point = workA0->items;
    best = NULL;
    bestDistance = 0x10000000;
    if (workA0 == NULL) {
      collisionFlags = 1;
      /* FAKEMATCH(dead): the event work slot copied onto itself, a second insn
         with no reload that keeps if-conversion from lifting the loop count
         above the null test; as the bare `collisionFlags = 1;`, 29 of 324
         instructions differ in tla-en. */
      Ram_HeapSlots->eventWork = Ram_HeapSlots->eventWork;
    } else {
      for (count = 0; count < 128; count++, point++) {
        if (point->used == 1) {
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
        // The slot index, a signed byte (ldrsb) that worka0.h's unk11 does not
        // yet type.
        if ((s8)best->unk11[0] != 0) {
          slot = &workA0->slots[(s8)best->unk11[0] - 1];
          slot->tail = (struct WorkA0Node *)1;
          slot->amplitude += 0x8000;
          if (slot->amplitude > 0x20000)
            slot->amplitude = 0x20000;
          object->watchedObject = (struct Actor *)best;
        }
        /* FAKEMATCH(dead): position.x copied onto itself before the copies,
           a store that stops gcse carrying the compare's x load past the
           pair block; as the plain x, y and z copies, 45 of 324 instructions
           differ in tla-en. */
        position.x = position.x;
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
  Actor_SelectAnimation(object, argument);
  Actor_SetAnimationSpeed(object, 8);
  sprite->unk00[23] = 4;
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
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    if (object->x == position.x && object->z == position.z)
      TurnToward(object, angle);
  }
  object->step++;
  return 1;
}
