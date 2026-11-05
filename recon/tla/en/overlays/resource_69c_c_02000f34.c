// Draft, differ 149 of 132, skeleton gap 11 (tla-en, 2026-11-01)
// Finds the object the leader is pushing: among objects 8 to 63 whose sprite
// shape is in the six-entry footprint table, the one whose footprint holds the
// cell one step ahead of the leader, when the leader is not in line with its
// near edge. Returns the object, with its id and the footprint's index; the
// leader's step direction is left in step. Shared with overlay 6a1 and others.
//
// Left: allocation, and the 11 removed and 20 added skeleton lines all follow
// from it. The listing has k r5, object r6, leader r7, then top ip, objects lr,
// z r8, left r10, x r9, index r11; right and bottom r0 and r1. This draft gives
// top r5, z r6, left r7, x ip, k lr, object r8, leader r10, objects r9. The
// greg order is bottom, right, top, z, left, x, k, object, leader, because the
// box values score about 1.0 to 1.35 (9 refs over 20 to 27 insns) and leader
// and object 0.63 (14 and 13 refs over 66 and 61). The listing's order puts
// them under objects and level with index, as if live about 120 insns, and no
// path in the listing's control flow keeps them live that long.
//
// Loop form is settled: both loops are goto, and objects is stepped by hand.
// A real inner loop, or a real outer loop around a goto inner one (k's constant
// set then makes it a biv of the outer loop), reduces k * 4 to adds #4, differ
// 178 and 183. Both arms hold their own *id store and return, as the listing.
// No gain from: a { z, x } struct (a DImode pair, r6 and r7), object position
// locals, a struct table (183), inline right and bottom (166), inline x and z
// (148, gap 23), s32 counters, swapped arms, nested range tests, one result
// variable, block-scope box locals, box values before x and z (202),
// *objects++ (165).
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field/event_work.h"

extern const u16 gData_02002478[];
extern const s8 gData_02002484[][4];
extern const s32 gData_0200249c[];

struct Actor *Actor_GetById(s32 id);

/// The whole-pixel half of a 16.16 coordinate, read as a halfword.
#define PIXELS(v) (((s16 *)&(v))[1])

struct Actor *Func_02000f34(s32 *step, s32 *id, s32 *footprint) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor **objects;
  struct Actor *object;
  u32 index;
  u32 k;
  s32 ahead;
  s32 x;
  s32 z;
  s32 left;
  s32 top;
  s32 right;
  s32 bottom;

  *step = leader->angle >> 12;
  index = 8;
  objects = &work->objects[8];
nextObject:
  object = *objects;
    k = 0;
  nextShape:
    {
      if (*(s16 *)((struct FieldSprite *)object->animation)->record !=
          gData_02002478[k])
        goto skip;
      ahead = gData_0200249c[*step];
      x = (PIXELS(leader->x) + (ahead >> 16)) >> 4;
      z = (PIXELS(leader->z) + (s16)ahead) >> 4;
      left = (PIXELS(object->x) + gData_02002484[k][0]) >> 4;
      top = (PIXELS(object->z) + gData_02002484[k][1]) >> 4;
      right = (PIXELS(object->x) + gData_02002484[k][2]) >> 4;
      bottom = (PIXELS(object->z) + gData_02002484[k][3]) >> 4;
      if (left > x || x >= right || top > z || z >= bottom)
        goto skip;
      *footprint = k;
      if (k & 1) {
        if (left == leader->x >> 20)
          goto skip;
        *id = index;
        return object;
      } else {
        if (top == leader->z >> 20)
          goto skip;
        *id = index;
        return object;
      }
    }
  skip:
    k++;
    if (k <= 5)
      goto nextShape;
  index++;
  objects++;
  if (index <= 63)
    goto nextObject;
  return 0;
}
