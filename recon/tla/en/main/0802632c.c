// Near miss, differ 56 of 535 (2026-10-26: movementAnimation set before
// collision and four probes copying y after z, 72 to 56): script command 0x23,
// from the listing and The Broken Seal's FieldObject_UpdatePlayerControl (its
// later layout: one probe ahead and four 22.5 and 45 degrees off, six retry
// headings). Moves an object on the field by the direction pad at a slow pace,
// stepping round walls and stopping at other solid objects (no pushing); with
// no event work it returns 0, a shortcut key pressed plays cue 113, otherwise
// the step takes the ground's height and the script advances. Left: the native
// keeps (u16)angle only in r1 for the first step and computes it again from the
// spilled angle (kept shifted left 16) at the second probe, where agscc keeps
// it in r7 from the top; registers shift from there, with r4 taking values
// the native keeps in r0 to r2. The debug L key is written as ☀️'s (heading
// set, the pushing loop skipped).
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"
#include "event_work.h"
#include "main_routine_work.h"
#include "actor.h"

struct FieldPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct PartyKeys {
  u8 unk000[0x23c];
  u16 dashKeys;
};

struct UnitHp {
  u8 unk00[0x38];
  s16 hp;
};

// The heading of each direction-pad combination, -1 for none.
extern const s16 gData_0802ec5c[16];

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 Func_0802d958(struct Actor *object, struct FieldPosition *position);
s32 Field_CheckSpheresOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b);
void Actor_SelectAnimation(struct Actor *object, s32 argument);
s32 Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);
void *Unit_Get(s32 owner);
s32 Func_0802d45c(s32 terrainId, s32 x, s32 z);

s32 Func_0802632c(struct Actor *object) {
  struct FieldPosition position;
  struct FieldPosition testPosition;
  struct FieldPosition contact;
  s16 angleOffsets[22];
  s32 collision;
  s16 heading;
  s32 angle;
  s32 movementAnimation;
  u16 direction;
  s32 angleIndex;
  s32 headingCount;
  struct Actor *other;
  s32 pressed;

  movementAnimation = 2;
  collision = 0;
  object->motionFlags = 4;
  if (gInput.held & ((struct PartyKeys *)&gMainRoutineWork)->dashKeys) {
    object->speedLimit = 0x8000;
    object->acceleration = 0x2000;
    movementAnimation = 5;
  } else {
    object->speedLimit = 0x8000;
    object->acceleration = 0x2000;
  }

  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  if ((u16)angle == 0xffff) {
    collision |= 4;
  } else {
    collision = 0;
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)angle, &position);
    if (gDebugMode != 0 && (gInput.held & 0x200)) {
      heading = angle;
    } else {
      if (Func_0802d958(object, &position) == 0) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)angle + 0x1000, &testPosition);
        if (Func_0802d958(object, &testPosition) == 0) {
          testPosition.x = object->x;
          testPosition.z = object->z;
          testPosition.y = object->y;
          Vector_AddPolarOffset(0x80000, (u16)angle - 0x1000, &testPosition);
          if (Func_0802d958(object, &testPosition) == 0) {
            testPosition.x = object->x;
            testPosition.z = object->z;
            testPosition.y = object->y;
            Vector_AddPolarOffset(0x80000, (u16)angle + 0x2000, &testPosition);
            if (Func_0802d958(object, &testPosition) == 0) {
              testPosition.x = object->x;
              testPosition.y = object->y;
              testPosition.z = object->z;
              Vector_AddPolarOffset(0x80000, (u16)angle - 0x2000,
                                    &testPosition);
              if (Func_0802d958(object, &testPosition) == 0) {
                heading = angle;
                goto check_objects;
              }
            }
          }
        }
      }
      direction = angle;
      headingCount = 0;
      angleOffsets[headingCount++] = direction + 0x1000;
      angleOffsets[headingCount++] = direction - 0x1000;
      angleOffsets[headingCount++] = direction + 0x2000;
      angleOffsets[headingCount++] = direction - 0x2000;
      angleOffsets[headingCount++] = direction + 0x3000;
      angleOffsets[headingCount++] = direction - 0x3000;
      for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
        heading = angleOffsets[angleIndex];
        position.x = object->x;
        position.z = object->z;
        position.y = object->y;
        Vector_AddPolarOffset(0x80000, (u16)heading, &position);
        if (Func_0802d958(object, &position) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading + 0x1000, &testPosition);
        if (Func_0802d958(object, &testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading - 0x1000, &testPosition);
        if (Func_0802d958(object, &testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading + 0x2000, &testPosition);
        if (Func_0802d958(object, &testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.z = object->z;
        testPosition.y = object->y;
        Vector_AddPolarOffset(0x80000, (u16)heading - 0x2000, &testPosition);
        if (Func_0802d958(object, &testPosition) == 0)
          goto check_objects;
      }
      position.x = object->x;
      position.y = object->y;
      position.z = object->z;
      collision |= 1;

    check_objects:
      contact.x = object->x;
      contact.y = object->y;
      contact.z = object->z;
      Vector_AddPolarOffset(0x40000, (u16)heading, &contact);
      other = Ram_HeapSlots->scriptObjects;
      for (angleIndex = 63; angleIndex >= 0; angleIndex -= 1, other++) {
        s32 radius;

        radius = object->radius - 2;
        if (other->script == 0)
          continue;
        if ((other->collisionFlags & 1) == 0)
          continue;
        if (other == object)
          continue;
        if (Field_CheckSpheresOverlap(&other->x, radius, &contact.x,
                                       other->radius - 2) < 0)
          continue;
        collision |= 2;
      }
    }
  }

  if (Ram_HeapSlots->eventWork != NULL) {
    struct EventWork *work = Ram_HeapSlots->eventWork;

    if (collision & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
  }
  if (collision != 0) {
    s32 action;

    action = 9;
    if (((struct UnitHp *)Unit_Get(gMainRoutineWork.currentOwner))->hp == 0)
      action = 22;
    Actor_SelectAnimation(object, action);
  } else {
    Actor_SelectAnimation(object, movementAnimation);
  }

  if (collision != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    if (collision & 3) {
      s32 difference;

      difference = (u16)angle - object->angle;
      difference = (s16)difference;
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->angle += difference;
    }
    object->action = 0;
    object->unk66 = 2;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    if (object->action != 0)
      object->action--;
  }

  if (Ram_HeapSlots->eventWork == NULL)
    return 0;
  pressed = 0;
  if ((gInput.pressed & gMainRoutineWork.keys234) ||
      (gInput.pressed & gMainRoutineWork.keys230) ||
      (gInput.pressed & gMainRoutineWork.keys236) ||
      (gInput.pressed & gMainRoutineWork.keys238) ||
      (gInput.pressed & gMainRoutineWork.keys23a))
    pressed = 1;
  if (pressed) {
    Sound_PlayFar(113);
  } else {
    object->terrainHeight =
        Func_0802d45c(object->terrainId, object->x, object->z);
    object->step++;
  }
  return 1;
}
