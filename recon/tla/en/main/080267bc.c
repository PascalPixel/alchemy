// Draft, differ 388 of 713: script command 0x24, a slow sibling of SC_PLAYER
// (Func_08027064): walks at one pixel a frame with a gentle acceleration
// (0xccc), the dash keys choosing only the running animation; steps 8
// pixels with the same probes, retries and pushing, but neither sets the
// velocity's heading nor records the held keys, and a refused step keeps
// the velocity. Structure follows the listing; left: register allocation
// and scheduling (frame 104 bytes, the native 100: one more spill slot).
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "event_work.h"
#include "party_state.h"
#include "object_runtime.h"

struct FieldPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct PartyKeys {
  u8 unk000[0x23c];
  u16 keys23c;
};

struct LeaderUnit {
  u8 unk00[0x38];
  s16 hp;
};

extern u8 gDebugMode;
// The heading of each direction-pad combination, -1 for none.
extern const s16 gData_0802ec5c[16];

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 Func_0802d87c(struct ObjectRuntime *object, struct FieldPosition *position);
s32 Runtime_CheckRadiusOverlap(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
s32 ScriptObject_CheckOverlap(struct ObjectRuntime *object, s32 *values);
s16 ArcTan2(s32 y, s32 x);
struct LeaderUnit *Unit_Get(s32 owner);
void ObjectDispatch_SelectAnimation(struct ObjectRuntime *object, s32 argument);
s32 Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
s32 Field_CheckConfiguredKeys(void);

// Two 16.16 coordinates fall in the same 16-pixel cell, rounding to nearest.
#define SAME_CELL(a, b) ((((a) - (b) + 0x80000) >> 20) == 0)

s32 Func_080267bc(struct ObjectRuntime *object) {
  struct FieldPosition position;
  struct FieldPosition testPosition;
  struct FieldPosition contact;
  s16 angleOffsets[22];
  s32 collision;
  s32 pushed;
  s32 movementAnimation;
  s32 angle;
  s32 heading;
  s32 angleIndex;
  s32 onObject;
  struct ObjectRuntime *other;
  struct EventWork *work;

  collision = 0;
  pushed = 0;
  movementAnimation = 2;
  if (gInput.held & ((struct PartyKeys *)&gPartyState)->keys23c) {
    object->speedLimit = 0x10000;
    object->acceleration = 0xccc;
    movementAnimation = 5;
  } else {
    object->speedLimit = 0x10000;
    object->acceleration = 0xccc;
  }

  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  if ((u16)angle == 0xffff) {
    collision |= 4;
    goto counted;
  }
  collision = 0;
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  Vector_AddPolarOffset(0x80000, (u16)angle, &position);
  if (object->priorityFlags & 0x40) {
    onObject = 0;
    other = (struct ObjectRuntime *)Ram_HeapSlots->scriptObjects;
    for (angleIndex = 0; angleIndex < 64; angleIndex++, other++) {
      s32 dy;

      if (other->script == NULL)
        continue;
      if (!(other->priorityFlags & 0x80))
        continue;
      if (other == object)
        continue;
      dy = object->y - other->y - 0x100000;
      if (dy < 0)
        dy = -dy;
      if (dy > 0x7ffff)
        continue;
      if (SAME_CELL(object->x, other->x) && SAME_CELL(object->z, other->z))
        onObject = 1;
      if (SAME_CELL(position.x, other->x) && SAME_CELL(position.z, other->z))
        goto clear;
    }
    if (onObject && !Func_0802d87c(object, &position)) {
      collision = 0;
      goto counted;
    }
  }
  if (gDebugMode != 0 && (gInput.held & 0x200))
    goto clear;
  if (!Func_0802d87c(object, &position)) {
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)angle + 0x1000, &testPosition);
    if (!Func_0802d87c(object, &testPosition)) {
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, (u16)angle - 0x1000, &testPosition);
      if (!Func_0802d87c(object, &testPosition)) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)angle + 0x2000, &testPosition);
        if (!Func_0802d87c(object, &testPosition)) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, (u16)angle - 0x2000, &testPosition);
          heading = angle;
          if (!Func_0802d87c(object, &testPosition))
            goto push_objects;
        }
      }
    }
  }
  {
    u16 direction = angle;

    angleOffsets[0] = direction + 0x1000;
    angleOffsets[1] = direction - 0x1000;
    angleOffsets[2] = direction + 0x2000;
    angleOffsets[3] = direction - 0x2000;
    angleOffsets[4] = direction + 0x3000;
    angleOffsets[5] = direction - 0x3000;
  }
  for (angleIndex = 0; angleIndex < 6; angleIndex++) {
    heading = angleOffsets[angleIndex];
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)heading, &position);
    if (Func_0802d87c(object, &position))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)heading + 0x1000, &testPosition);
    if (Func_0802d87c(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)heading - 0x1000, &testPosition);
    if (Func_0802d87c(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)heading + 0x2000, &testPosition);
    if (Func_0802d87c(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)heading - 0x2000, &testPosition);
    if (!Func_0802d87c(object, &testPosition))
      goto push_objects;
  }
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  collision |= 1;
  goto push_objects;

clear:
  collision = 0;
  goto counted;

push_objects:
  contact.x = object->x;
  contact.y = object->y;
  contact.z = object->z;
  Vector_AddPolarOffset(0x40000, (u16)heading, &contact);
  other = (struct ObjectRuntime *)Ram_HeapSlots->scriptObjects;
  for (angleIndex = 63; angleIndex >= 0; angleIndex--, other++) {
    s32 radius;

    radius = object->radius - 2;
    if (other->script == NULL)
      continue;
    if (!(other->collisionFlags & 1))
      continue;
    if (other == object)
      continue;
    if (Runtime_CheckRadiusOverlap(&other->x, radius, &contact.x,
                                   other->radius - 2) < 0)
      continue;
    if ((other->collisionFlags & 2) && other->movementState == 0) {
      u16 direction;

      direction = ArcTan2(other->z - object->z, other->x - object->x);
      testPosition.x = other->x;
      testPosition.y = other->y;
      testPosition.z = other->z;
      Vector_AddPolarOffset(0x4000, direction, &testPosition);
      if (!ScriptObject_CheckOverlap(other, &testPosition.x)) {
        testPosition.x = other->x;
        testPosition.y = other->y;
        testPosition.z = other->z;
        Vector_AddPolarOffset(0xa0000, direction, &testPosition);
        if (!Func_0802d87c(other, &testPosition)) {
          testPosition.x = other->x;
          testPosition.y = other->y;
          testPosition.z = other->z;
          Vector_AddPolarOffset(0xa0000, direction + 0x1000, &testPosition);
          // The same probe is tested twice.
          if (!Func_0802d87c(other, &testPosition) &&
              !Func_0802d87c(other, &testPosition)) {
            testPosition.x = other->x;
            testPosition.y = other->y;
            testPosition.z = other->z;
            Vector_AddPolarOffset(0xa0000, direction - 0x1000, &testPosition);
            if (!Func_0802d87c(other, &testPosition)) {
              Vector_AddPolarOffset(0x4000, direction,
                                    (struct FieldPosition *)&other->x);
              other->targetX = 0x80000000;
              other->targetY = 0x80000000;
              other->targetZ = 0x80000000;
              pushed |= 1;
              continue;
            }
          }
        }
      }
    }
    collision |= 2;
  }
  if (collision == 0 && pushed != 0) {
    object->speedLimit = 0x4000;
    object->acceleration = 0x2000;
  }

counted:
  work = Ram_HeapSlots->eventWork;
  if (work != NULL) {
    if (collision & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
  }
  if (pushed != 0) {
    ObjectDispatch_SelectAnimation(object, 8);
  } else if (collision != 0) {
    s32 action;

    action = 9;
    if (Unit_Get(gPartyState.currentOwner)->hp == 0)
      action = 22;
    ObjectDispatch_SelectAnimation(object, action);
  } else {
    ObjectDispatch_SelectAnimation(object, movementAnimation);
  }

  if (collision != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    if (collision & 3) {
      s32 difference;

      difference = (s16)((u16)angle - object->angle);
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->angle += difference;
    }
    object->action = 0;
    object->unk66 = 2;
  } else {
    Object_SetMoveTarget(object, position.x, position.y, position.z);
    if (object->action != 0)
      object->action--;
  }
  Field_CheckConfiguredKeys();
  object->step++;
  return 1;
}
