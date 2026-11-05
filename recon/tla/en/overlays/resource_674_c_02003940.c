// Draft, differ 136 of 132, skeleton gap 30 (tla-en, 2026-11-01)
// Overlay 674's copy of overlay 668's Func_020043a8 and 662's Func_0200033c:
// finds the pushable object in front of the leader (see 668's header), in the
// same goto form.
// Left: nearly all allocation; the extra 10 instructions are movs for k, object
// and leader in high registers. The native gives r5, r6, r7 to k, object,
// leader, then ip top, lr entry, r8 z, r10 left, r9 x, r11 i. Here the lreg dump
// ranks (flow's refs, log2(refs) * refs / live) top 1.35, z 1.13, left 1.08,
// x 1.0, k 0.81, object 0.64, leader 0.64, entry 0.23, i 0.21, because block 3
// is at loop depth 2. Counting each ref once gives the native's order exactly,
// entry between top and z included. Both loops are natural whatever the C says
// (a single entry and one latch in the listing), so no plain source removes the
// weight. XXX: why these refs carry no loop weight; this is a compiler question,
// out of a draft's reach (flow.c drops the weight only under optimize_size).
// Measured (differ/gap, draft goto form 136/30): for + for 161/39, work->objects[i]
// in for + for 159/37, outer for inner goto 167/43, the same with objects[i]
// 165/41, inner for outer goto 175/41, do + do 159/37, shared offset local 149/31,
// object pixels in locals 149/33, s8 *box row 174/44, both 180/48, right and
// bottom unnamed 152/44. No change: edges in an inner block, goto next for the
// box test.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"

/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

extern u16 gData_02004070[];
extern s8 gData_0200407c[][4];
extern s32 gData_02004094[];

struct Actor *Actor_GetById(s32 id);

struct Actor *Func_02003940(s32 *direction, s32 *slot, s32 *kind) {
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
      gData_02004070[k]) {
    x = (PIXELS(leader->x) + (gData_02004094[*direction] >> 16)) >> 4;
    z = (PIXELS(leader->z) + (s16)gData_02004094[*direction]) >> 4;
    left = (PIXELS(object->x) + gData_0200407c[k][0]) >> 4;
    top = (PIXELS(object->z) + gData_0200407c[k][1]) >> 4;
    right = (PIXELS(object->x) + gData_0200407c[k][2]) >> 4;
    bottom = (PIXELS(object->z) + gData_0200407c[k][3]) >> 4;
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
