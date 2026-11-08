// Matches: differ 0 (tla-en, 2026-11-08)
// Also differ 0 in tla-ja, tla-de, tla-es, tla-fr and tla-it (whole-function
// diff, 713 instructions each). Three tagged dead devices remain (S2): two
// tests and the debug test's store of collision's 0; each is needed and
// leaves no instruction. The ROM-kept stores of 0 to collision are // DEAD:.
// How it matches (2026-11-08, from 31): gInput is read directly (the keys
// pointer is gone, so no mov sl); the debug test has its own copy of
// "collision = 0; goto counted", whose reload takes r1 in the debug test and
// puts the first two probes on the native's reload registers (round robin,
// reload1.c last_spill_reg: object r2, angle r3, 0x1000 r0). jump2's
// cross-jumping then folds that copy into the clear block, so the debug test
// branches to clear as in the listing. With only the scan loop jumping to
// clear, cleanup_cfg would pull the clear block into the loop and loop.c move
// it out after the straight probes (183): the dead headingCount test is a
// second reference that keeps it in place until CSE folds the test away.
// onObject is cleared inside the block, after the scan pointer, as the listing
// schedules it; alone that gives it r4 and the bound r6 (12: onObject, 4 refs
// over 100 insns, outranks the bound's 3 refs over a doubled 92). The dead test
// of onObject after the block makes onObject live on the path that skips the
// block, so it crosses calls, takes r6 and leaves r4 to the bound; the test
// itself is gone by jump2.
// Tried for those two without gain: an empty asm reading onObject after the
// block (also 0, but a barrier), onObject = onObject (12), an empty asm (12);
// the bound compared in both arms of the absolute difference (47: cross-jumping
// keeps both compares), a scan counter of its own (117), a goto-built scan loop
// (96), onObject zeroed both before the step and in the block (24, 39: CSE
// copies collision's zero); a "cleared:" label between clear's store and its
// jump (185: jump threading removes it), the debug store before goto clear (40:
// dead before reload), pushed = 0 there (32: two more instructions), the debug
// test as the else of an if over the probes (40, or 121 with the scan's own
// copy: the two copies take r1 and r2 and do not merge), and the key test on
// (held >> 9) & 1 (44).
// Twin of The Broken Seal's Actor_UpdatePlayerControl
// (games/tbs/src/field/field/player_control.c), script command 0x24 in both
// games: the same 8-pixel step with five probes, the same six fallback
// headings, the same push test with its probe tested twice, the same blocked
// count, animations and turn. This game drops the debug stalls, the fast walk,
// the velocity rewrite and the footprints, walks at one pixel a frame with a
// gentle acceleration (0xccc), and adds a still-object check on priorityFlags
// bits 6 and 7 before the step. The angle is a u16 and the heading an s16 (the
// twin's s32 angle, differ 627).
// From the twin: headingCount filling angleOffsets, which keeps the loop's
// compare as cmp 6 and blt; the heading set after the last probe; the absolute
// difference written as 0x100000 - delta; the bound in a local so it stays a
// hoisted 0x7ffff (a literal compares as >= 0x80000); the clear block between
// the loop and the blocked block. Differ 191 before.
// Earlier (2026-11-06 to 08, at 48 and 31, before the two dead tests): a keys
// pointer in sl made the probes match at the cost of a mov sl; onObject cleared
// before the step had r4 and r6 right but the clear before the priorityFlags
// branch; permutes of 5 and 9 minutes found nothing below 31.
// Tried without gain at 48: every order of the three opening values
// (movementAnimation first is the best), the mask operands swapped, a pointer
// to the keys, an interpreter copy of the argument, declaration order and scope
// of onObject, the bound, direction and headingCount, the twin's declaration
// order, and a u8 onObject; onObject as u32, s16 or u16, an unsigned bound,
// the bound test nested, the onObject test nested or as != 0 and == 0, the
// debug test as an else-if, as != 0 or with its mask swapped, the probe angle
// as 0x1000 + angle, uncast or unsigned (all 48). A forced temporary for the
// first probe's angle, set before its copies, gives 32 (the second probe
// matches, the first moves further off); an s32 angle gives 625.
// A 5-minute permute (2026-11-06) found nothing below 48 (best 49, a forced
// temporary).
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
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
s32 Func_0802d87c(struct Actor *object, struct FieldPosition *position);
s32 Field_CheckSpheresOverlap(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
s32 ScriptObject_CheckOverlap(struct Actor *object, s32 *values);
s16 ArcTan2(s32 y, s32 x);
struct LeaderUnit *Unit_Get(s32 owner);
void Actor_SelectAnimation(struct Actor *object, s32 argument);
s32 Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);
s32 Field_CheckConfiguredKeys(void);

// Two 16.16 coordinates fall in the same 16-pixel cell, rounding to nearest.
#define SAME_CELL(a, b) ((((a) - (b) + 0x80000) >> 20) == 0)

s32 Func_080267bc(struct Actor *object) {
  struct FieldPosition position;
  struct FieldPosition testPosition;
  struct FieldPosition contact;
  s16 angleOffsets[22];
  s32 collision;
  s32 pushed;
  s32 movementAnimation;
  u16 angle;
  s16 heading;
  s32 angleIndex;
  s32 onObject;
  struct Actor *other;
  struct EventWork *work;

  movementAnimation = 2;
  collision = 0;
  pushed = 0;
  if (gInput.held & ((struct PartyKeys *)&gMainRoutineWork)->keys23c) {
    object->speedLimit = 0x10000;
    object->acceleration = 0xccc;
    movementAnimation = 5;
  } else {
    object->speedLimit = 0x10000;
    object->acceleration = 0xccc;
  }

  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  if (angle == 0xffff) {
    collision |= 4;
    goto counted;
  }
  // DEAD: collision is already 0 here.
  collision = 0;
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  Vector_AddPolarOffset(0x80000, angle, &position);
  if (object->priorityFlags & 0x40) {
    s32 withinHalfCell;

    onObject = 0;
    other = (struct Actor *)Ram_HeapSlots->scriptObjects;
    withinHalfCell = 0x7ffff;
    for (angleIndex = 0; angleIndex < 64; angleIndex++, other++) {
      s32 delta;
      s32 dy;

      if (other->script == NULL)
        continue;
      if (!(other->priorityFlags & 0x80))
        continue;
      if (other == object)
        continue;
      delta = object->y - other->y;
      dy = delta - 0x100000;
      if (dy < 0)
        dy = 0x100000 - delta;
      if (dy > withinHalfCell)
        continue;
      if (SAME_CELL(object->x, other->x) && SAME_CELL(object->z, other->z))
        onObject = 1;
      if (SAME_CELL(position.x, other->x) && SAME_CELL(position.z, other->z))
        goto clear;
    }
    if (onObject && !Func_0802d87c(object, &position)) {
      // DEAD: collision is already 0 here.
      collision = 0;
      goto counted;
    }
  }
  /* FAKEMATCH(dead): the test that sets onObject to 1 when it is nonzero kept
     (nothing reads onObject after it, and it is unset when priorityFlags bit 6
     is clear); as no test, 12 of 713 instructions differ in tla-en. */
  if (onObject)
    onObject = 1;
  if (gDebugMode != 0 && (gInput.held & 0x200)) {
    /* FAKEMATCH(dead): a store of the 0 collision already holds, which
       cross-jumping folds into clear; as goto clear, 36 of 713 instructions
       differ in tla-en. */
    collision = 0;
    goto counted;
  }
  if (!Func_0802d87c(object, &position)) {
    testPosition.x = object->x;
    testPosition.y = object->y;
    testPosition.z = object->z;
    Vector_AddPolarOffset(0x80000, angle + 0x1000, &testPosition);
    if (!Func_0802d87c(object, &testPosition)) {
      testPosition.x = object->x;
      testPosition.y = object->y;
      testPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, angle - 0x1000, &testPosition);
      if (!Func_0802d87c(object, &testPosition)) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, angle + 0x2000, &testPosition);
        if (!Func_0802d87c(object, &testPosition)) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, angle - 0x2000, &testPosition);
          if (!Func_0802d87c(object, &testPosition)) {
            heading = angle;
            goto push_objects;
          }
        }
      }
    }
  }
  {
    s32 headingCount = 0;

    angleOffsets[headingCount++] = angle + 0x1000;
    angleOffsets[headingCount++] = angle - 0x1000;
    angleOffsets[headingCount++] = angle + 0x2000;
    angleOffsets[headingCount++] = angle - 0x2000;
    angleOffsets[headingCount++] = angle + 0x3000;
    angleOffsets[headingCount++] = angle - 0x3000;
    /* FAKEMATCH(dead): the never-true test of headingCount kept; as no test,
       183 of 713 instructions differ in tla-en. */
    if (headingCount == 0)
      goto clear;
    for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
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
  }
  goto blocked;

clear:
  // DEAD: collision is already 0 here.
  collision = 0;
  goto counted;

blocked:
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  collision |= 1;

push_objects:
  contact.x = object->x;
  contact.y = object->y;
  contact.z = object->z;
  Vector_AddPolarOffset(0x40000, (u16)heading, &contact);
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
    Actor_SelectAnimation(object, 8);
  } else if (collision != 0) {
    s32 action;

    action = 9;
    if (Unit_Get(gMainRoutineWork.currentOwner)->hp == 0)
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

      difference = (s16)(angle - object->angle);
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
  Field_CheckConfiguredKeys();
  object->step++;
  return 1;
}
