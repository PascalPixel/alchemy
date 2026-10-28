// Draft, differ 140 of 132 (was 157): finds the pushable object the leader
// faces among objects 8 to 63, its kind and slot. Rewritten from the listing
// as two goto loops (the native has no loop optimization: it reloads the
// sprite id and the tables on each pass and recomputes the kind's row), the
// outer one stepping a pointer through the event work's object table as the
// native does in lr. The instructions now follow the native's nearly row for
// row. Left: registers: the native gives r7, r6 and r5 to the leader, the
// object and the kind and r9, r8, r10 and ip to the point and box edges; here
// the edges take the low registers (global.c ranks them first). Measured: the
// edges declared in the inner block or in another order, the counters signed,
// either loop as a for loop (154 to 179); permute found nothing.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "event_work.h"

/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

extern u16 gData_02001968[];
extern s8 gData_02001974[][4];
extern s32 gData_0200198c[];

struct ObjectRuntime *Object_GetById(s32 id);

struct ObjectRuntime *Func_0200033c(s32 *direction, s32 *slot, s32 *kind) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  struct ObjectRuntime **entry;
  struct ObjectRuntime *object;
  u32 i, k;
  s32 x, z, left, top, right, bottom;

  *direction = leader->angle >> 12;
  i = 8;
  entry = &work->objects[8];
outer:
  object = *entry;
  k = 0;
inner:
  if (((struct BattleAnimationState *)object->animation)->value28[0] ==
      gData_02001968[k]) {
    x = (PIXELS(leader->x) + (gData_0200198c[*direction] >> 16)) >> 4;
    z = (PIXELS(leader->z) + (s16)gData_0200198c[*direction]) >> 4;
    left = (PIXELS(object->x) + gData_02001974[k][0]) >> 4;
    top = (PIXELS(object->z) + gData_02001974[k][1]) >> 4;
    right = (PIXELS(object->x) + gData_02001974[k][2]) >> 4;
    bottom = (PIXELS(object->z) + gData_02001974[k][3]) >> 4;
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
