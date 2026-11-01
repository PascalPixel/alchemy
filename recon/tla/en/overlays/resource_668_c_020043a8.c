// Draft, differ 138 of 132, skeleton gap 32 (tla-en, 2026-11-01)
// Finds the pushable object in front of the leader: records the leader's
// direction (its angle's high four bits), then for objects 8 to 63 (the event
// work's object table) and the six kinds of gData_02004ca4 tests the point one
// step ahead (gData_02004cc8, packed offsets by direction) against the kind's
// box (gData_02004cb0); a hit not already level with the leader on the kind's
// axis records the kind and the object's number and is returned, else 0. The
// same listing as overlay 674's Func_02003940 and 662's Func_0200033c.
// Left: the whole gap is allocation; the statements already follow the listing
// row for row. The native gives r5, r6, r7 to k, object, leader, then ip, lr,
// r8, r10, r9, r11 to top, entry, z, left, x, i. Here top, z, left take r5 to
// r7 and every use of k, object and leader gains a mov. flow.c weights a ref
// by loop depth + 1, and global.c ranks refs * log2(refs) / live length; with
// the weights of a depth-2 body top, z, left, x rank 1.35 to 1.0 and k, object,
// leader, entry 0.8 to 0.45. Counting every ref as 1 gives 0.21 k, 0.20 object,
// 0.18 leader, 0.15 top, 0.13 entry, 0.125 z, 0.12 left, 0.11 x, i last:
// exactly the native's order. So the native's flow saw no loop depth here,
// although both loops are natural in this C; nothing plain in the source moves
// these ranks. XXX: why the native's refs carry no loop weight; a matched
// nested loop whose allocation depends on those weights would settle it.
// Measured (gap): for loops 39 (k * 4 strength-reduced, tables hoisted), outer
// for or do with inner goto 43, inner do with outer goto 43, right and bottom
// not named 46; the edges in an inner block or declared first change nothing.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"

/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

extern u16 gData_02004ca4[];
extern s8 gData_02004cb0[][4];
extern s32 gData_02004cc8[];

struct Actor *Actor_GetById(s32 id);

struct Actor *Func_020043a8(s32 *direction, s32 *slot, s32 *kind) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor **entry;
  struct Actor *object;
  u32 i, k;
  s32 x, z, left, top, right, bottom;

  *direction = leader->angle >> 12;
  i = 8;
  entry = &work->objects[8];
outer:
  object = *entry;
  k = 0;
inner:
  if (((struct ObjectAnimationState *)object->animation)->value28[0] ==
      gData_02004ca4[k]) {
    x = (PIXELS(leader->x) + (gData_02004cc8[*direction] >> 16)) >> 4;
    z = (PIXELS(leader->z) + (s16)gData_02004cc8[*direction]) >> 4;
    left = (PIXELS(object->x) + gData_02004cb0[k][0]) >> 4;
    top = (PIXELS(object->z) + gData_02004cb0[k][1]) >> 4;
    right = (PIXELS(object->x) + gData_02004cb0[k][2]) >> 4;
    bottom = (PIXELS(object->z) + gData_02004cb0[k][3]) >> 4;
    if (left <= x && x < right && top <= z && z < bottom) {
      *kind = k;
      if (k & 1) {
        if (left != leader->x >> 20) {
          *slot = i;
          return object;
        }
      } else {
        if (top != leader->z >> 20) {
          *slot = i;
          return object;
        }
      }
    }
  }
  k++;
  if (k <= 5)
    goto inner;
  i++;
  entry++;
  if (i <= 63)
    goto outer;
  return NULL;
}
