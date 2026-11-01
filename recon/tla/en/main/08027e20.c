// Matches: differ 0 (tla-en, 2026-11-01)
// Script command 0x1f, a relative of Actor_UpdatePlayerControl (and of
// SC_PLAYER, Func_08027064): the statements, declarations and headingCount
// table are the twin's, minus running and footprints. It takes two tagged
// one-pass devices; volatile, wrapper, temporary and eight plain reshapes did
// not move those sites. The one at the table also keeps the block that clears
// collision inside the first loop placed after the retry loop.
// It has longer reach: steps 19 pixels ahead (with the four probes 22.5 and
// 45 degrees off and six retry headings), pushes from 15 pixels and tests 21
// pixels past a pushed object; with flag 0x17f, B moves
// at 4 pixels a frame. The first shortcut key posts request 252 in the event
// work; turns at most 0x400 a step, and an object with one sprite reads the
// world map's ground under it afterwards. Structure and frame follow the
// listing, the angle a u16 as in Func_08027064; as s32 a second copy spills.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "map.h"
#include "event_work.h"
#include "main_routine_work.h"
#include "actor.h"
#include "iwram_call.h"

struct FieldPosition {
  s32 x;
  s32 y;
  s32 z;
};

extern u8 gDebugMode;
// The heading of each direction-pad combination, -1 for none.
extern const s16 gData_0802ec5c[16];

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 Func_0802d87c(struct Actor *object, struct FieldPosition *position);
s32 Field_CheckSpheresOverlap(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
s32 ScriptObject_CheckOverlap(struct Actor *object, s32 *values);
s16 ArcTan2(s32 y, s32 x);
s32 GameFlag_Test(s32 flag);
void Actor_SelectAnimation(struct Actor *object, s32 argument);
s32 Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);
s32 FixedSqrt(s32 value);
s32 GetWorldMapCollision(struct WorldPosition *position);
s32 Field_CheckConfiguredKeys(void);

// Two 16.16 coordinates fall in the same 16-pixel cell, rounding to nearest.
#define SAME_CELL(a, b) ((((a) - (b) + 0x80000) >> 20) == 0)

s32 Func_08027e20(struct Actor *interpreter) {
  struct Actor *object = interpreter;

  struct FieldPosition position;
  struct FieldPosition testPosition;
  struct FieldPosition contact;
  s16 angleOffsets[22];
  s32 collision;
  s32 pushed;
  s16 heading;
  u16 angle;
  u16 direction;
  s32 angleIndex;
  s32 headingCount;
  s32 onObject;
  s32 difference;
  struct Actor *other;
  struct EventWork *work;

  collision = 0;
  pushed = 0;
  /* FAKEMATCH(one-pass): the empty block keeps gDebugMode's address load after
   * the two stores; as no block, 2 of 827 instructions differ in tla-en. */
  do {
  } while (0);
  // Debug: holding L spins through four empty loops, a short stall each
  // frame.
  if (gDebugMode != 0) {
    s32 delayKeyMask;

    delayKeyMask = 0x200;
    if (gInput.held & delayKeyMask) {
      s32 delayIterations;

      delayIterations = delayKeyMask;
    wait_a:
      --delayIterations;
      if (delayIterations != 0)
        goto wait_a;
      delayIterations = 95;
    wait_b:
      delayIterations--;
      if (delayIterations >= 0)
        goto wait_b;
      delayIterations = 63;
    wait_c:
      delayIterations--;
      if (delayIterations >= 0)
        goto wait_c;
      delayIterations = 63;
    wait_d:
      delayIterations--;
      if (delayIterations >= 0)
        goto wait_d;
    }
  }
  object->speedLimit = 0x10000;
  object->acceleration = 0x4000;
  if (GameFlag_Test(0x17f) != 0 && (gInput.held & 2)) {
    object->speedLimit = 0x40000;
    object->acceleration = 0x10000;
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
  Vector_AddPolarOffset(0x130000, (u16)angle, &position);
  if (object->priorityFlags & 0x40) {
    onObject = 0;
    other = (struct Actor *)Ram_HeapSlots->scriptObjects;
    for (angleIndex = 0; angleIndex < 64; angleIndex++, other++) {
      s32 dy;

      if (other->script == NULL)
        continue;
      if (!(other->priorityFlags & 0x80))
        continue;
      if (other == object)
        continue;
      dy = object->y - other->y;
      if ((dy - 0x100000 < 0 ? -(dy - 0x100000) : dy - 0x100000) > 0x7ffff)
        continue;
      if (SAME_CELL(object->x, other->x) && SAME_CELL(object->z, other->z))
        onObject = 1;
      if (SAME_CELL(position.x, other->x) && SAME_CELL(position.z, other->z)) {
        collision = 0;
        heading = angle;
        goto counted;
      }
    }
    if (onObject && !Func_0802d87c(object, &position)) {
      collision = 0;
      heading = angle;
      goto counted;
    }
  }
  if (gDebugMode != 0 && (gInput.held & 0x200)) {
    collision = 0;
    heading = angle;
    goto counted;
  }
  if (!Func_0802d87c(object, &position)) {
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x130000, (u16)angle + 0x1000, &testPosition);
    if (!Func_0802d87c(object, &testPosition)) {
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset(0x130000, (u16)angle - 0x1000, &testPosition);
      if (!Func_0802d87c(object, &testPosition)) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x130000, (u16)angle + 0x2000, &testPosition);
        if (!Func_0802d87c(object, &testPosition)) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset(0x130000, (u16)angle - 0x2000, &testPosition);
          if (!Func_0802d87c(object, &testPosition)) {
            heading = angle;
            goto push_objects;
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
  /* FAKEMATCH(one-pass): the empty block keeps the last store before the counter
   * and places the first loop's exit block after this loop; as no block, 243 of
   * 827 instructions differ in tla-en. */
  do {
  } while (0);
  for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
    heading = angleOffsets[angleIndex];
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x130000, (u16)heading, &position);
    if (Func_0802d87c(object, &position))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x130000, (u16)heading + 0x1000, &testPosition);
    if (Func_0802d87c(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x130000, (u16)heading - 0x1000, &testPosition);
    if (Func_0802d87c(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x130000, (u16)heading + 0x2000, &testPosition);
    if (Func_0802d87c(object, &testPosition))
      continue;
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x130000, (u16)heading - 0x2000, &testPosition);
    if (!Func_0802d87c(object, &testPosition))
      goto push_objects;
  }
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  collision |= 1;

push_objects:
  contact.x = object->x;
  contact.y = object->y;
  contact.z = object->z;
  Vector_AddPolarOffset(0xf0000, (u16)heading, &contact);
  other = (struct Actor *)Ram_HeapSlots->scriptObjects;
  for (angleIndex = 63; angleIndex >= 0; angleIndex--, other++) {
    s32 radius;

    radius = object->radius - 2;
    if (other->script == NULL)
      continue;
    if (!(other->collisionFlags & 1))
      continue;
    if (other == object)
      continue;
    if (Field_CheckSpheresOverlap(&other->x, radius, &contact.x,
                                   other->radius - 2) < 0)
      continue;
    if ((other->collisionFlags & 2) && other->movementState == 0) {
      direction = ArcTan2(other->z - object->z, other->x - object->x);
      testPosition.x = other->x;
      testPosition.y = other->y;
      testPosition.z = other->z;
      heading = direction;
      Vector_AddPolarOffset(0x4000, direction, &testPosition);
      if (!ScriptObject_CheckOverlap(other, &testPosition.x)) {
        testPosition.x = other->x;
        testPosition.y = other->y;
        testPosition.z = other->z;
        Vector_AddPolarOffset(0x150000, direction, &testPosition);
        if (!Func_0802d87c(other, &testPosition)) {
          testPosition.x = other->x;
          testPosition.y = other->y;
          testPosition.z = other->z;
          Vector_AddPolarOffset(0x150000, direction + 0x1000, &testPosition);
          // The same probe is tested twice.
          if (!Func_0802d87c(other, &testPosition) &&
              !Func_0802d87c(other, &testPosition)) {
            testPosition.x = other->x;
            testPosition.y = other->y;
            testPosition.z = other->z;
            Vector_AddPolarOffset(0x150000, direction - 0x1000, &testPosition);
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
    work->heldKeys = gInput.held;
  }
  if (pushed != 0 || collision != 0)
    Actor_SelectAnimation(object, 1);
  else
    Actor_SelectAnimation(object, 2);

  if (gInput.pressed & gMainRoutineWork.keys230) {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->unk16a = 252;
    collision |= 4;
  }

  if (collision != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    object->velocityX = 0;
    object->velocityZ = 0;
    if (collision & 3) {
      difference = (s16)((u16)angle - object->angle);
      if (difference > 0x1000)
        difference = 0x400;
      if (difference < -0x1000)
        difference = -0x400;
      object->angle += difference;
    }
    object->action = 0;
    object->unk66 = 2;
  } else {
    s32 speed;

    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)heading,
                          (struct FieldPosition *)&object->velocityX);
    if (object->action != 0)
      object->action--;
    object->actionFlags &= ~1;
    difference = (s16)((u16)angle - object->angle);
    if (difference > 0x1000)
      difference = 0x400;
    if (difference < -0x1000)
      difference = -0x400;
    object->angle += difference;
  }
  if (object->animationKind == 1)
    GetWorldMapCollision((struct WorldPosition *)&object->x);
  Field_CheckConfiguredKeys();
  object->step++;
  return 1;
}
