// Matches: differ 0 (tla-en, 2026-11-08)
// Also differ 0 in tla-ja, tla-de, tla-es, tla-fr and tla-it. Heap slot 0xa0
// is read through worka0.h's placeholder types (no evidence yet for what its
// points are). Two tagged dead self-copies make the match:
// - In the null work block, `Ram_HeapSlots->eventWork` copied onto itself
//   gives the block a second insn, so ifcvt's case 2 (then block over
//   BRANCH_COST, 1 for Thumb at -O2) keeps the loop count's 127 below the
//   test; addressed off a fresh constant in a low register, it needs no
//   reload, so reload's round-robin of r0 to r3 puts the 127 in r0 right
//   after collisionFlags' r3, as the native does. The old pad,
//   `position.y = position.y`, reloaded &position (r9) into r0 and pushed the
//   127 into r1 (differ 8, any position member or order: 8 or 22); a
//   self-copy through object folds away at expand (33); through gInput, cse
//   reuses the opening &gInput and moves it to r5 (4); the slot read onto
//   itself 53; an empty asm there also gives 0, but is a barrier.
// - Before the best point's copies, position.x copied onto itself is a store
//   that kills best->x's availability, so gcse's PRE no longer inserts the
//   x load at the pair block's end and reuses the compare's (copies x, y, z
//   with no kill, 45); copying y first instead (the old form, no device)
//   leaves only the copy order (4). A struct copy of the position emits
//   ldmia/stmia (185); an inline copy helper on &best->position keeps an
//   adds, as cse never folds the +4 into the loads (35); one on best is PRE'd
//   as before (49).
// Earlier, differ 8 of 324 (tla-en, 2026-11-08): best and bestDistance set in
// the other order 18; the point pointer set inside the else 14; count <= 127,
// u32 (96) or redeclared 8; a once-only block, `|=`, goto or `else if` in
// place of the null pad 33; the sibling's `goto stop` into the arrival block
// 27 (a beq to the shared block, not the native's own null block).
// Earlier, differ 26 of 324 (tla-en, 2026-11-06): the null test read
// `if (workA0 != NULL) { ... } else collisionFlags = 1;`, so the loop count's
// start stays after the test; left: the null path's block sits after the
// point loop instead of right after the test (with `workA0 == NULL` first,
// ifcvt's case 2 lifts the 127 above the test, differ 33: the native's else
// block must hold something that stops it), the knock-on branch and literal
// offsets, and the y copy's load before x's. Copy orders x,y,z 57; x,z,y 61;
// z,y,x 30; y,z,x and z,x,y 26. Counting down by hand, differ 104; skipping
// the loop into the shared `best == NULL` test, differ 27.
// Draft, differ 36 of 324 (2026-10-26, 73 before): the best point's position
// copied y first, which gives the native's registers and instruction count;
// left: the loop count's start (127) hoisted above the work's null test
// (if-conversion here, not in the native), the opening position's address
// built in one step, and the x and y copies' loads in the other order.
// Earlier notes follow.
// Near miss, differ 81 of 324: script command 0x1d, the sibling of
// Func_080292fc: moves an object to the nearest slot-0xa0 point within 32
// pixels of the direction pad's step, turning it toward the pad by at most
// 0x1000 when refused or arrived. Remaining: the angle table's address loads
// before the held keys natively (an empty asm on the table gets it after
// the input's address, differ 79); register choice in the best point's
// position test (sl as base, the native's r1); sched2 order around the
// copies, as in Func_08025698. The spilled locals are declared in reverse
// stack order and the point loop counts up, as in Func_080292fc.
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
