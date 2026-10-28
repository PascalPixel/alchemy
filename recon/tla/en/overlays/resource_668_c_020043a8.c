// Draft, differ 140 of 132 (was 159): finds the pushable object in front of
// the leader: records the leader's direction (its angle's high four bits),
// then for objects 8 to 63 (the event work's object table) and the six kinds
// of gData_02004ca4 tests the point one step ahead (gData_02004cc8, packed
// offsets by direction) against the kind's box (gData_02004cb0); a hit not
// already level with the leader on the kind's axis records the kind and the
// object's number and is returned, else 0. The same listing as overlay 674's
// Func_02003940 and 662's Func_0200033c, and written as the latter's draft
// (two goto loops, a pointer stepping through the object table). Left: as
// that draft, the box edges take the low registers the native gives the
// leader, the object and the kind.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "event_work.h"

/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

extern u16 gData_02004ca4[];
extern s8 gData_02004cb0[][4];
extern s32 gData_02004cc8[];

struct ObjectRuntime *Object_GetById(s32 id);

struct ObjectRuntime *Func_020043a8(s32 *direction, s32 *slot, s32 *kind) {
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
