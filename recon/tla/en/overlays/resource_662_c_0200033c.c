// Draft, differ 157 of 132: finds the pushable object the leader faces among
// objects 8 to 63, its kind and slot. Left: the native reloads the sprite id
// and the table on each pass and keeps the leader in r7.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "event_work.h"

/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

extern u16 gData_02001968[];
extern s8 gData_02001974[][4];
extern s32 gData_0200198c[];

struct ObjectRuntime *Object_GetById(s32 id);

struct ObjectRuntime *Func_0200033c(s32 *direction, s32 *slot, s32 *kind) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  u32 i, k;

  *direction = leader->angle >> 12;
  for (i = 8; i < 64; i++) {
    struct ObjectRuntime *object = work->objects[i];

    for (k = 0; k < 6; k++) {
      s32 x, z, left, top, right, bottom;

      if (((struct BattleAnimationState *)object->animation)->value28[0] !=
          gData_02001968[k])
        continue;
      x = (PIXELS(leader->x) + (gData_0200198c[*direction] >> 16)) >> 4;
      z = (PIXELS(leader->z) + (s16)gData_0200198c[*direction]) >> 4;
      left = (PIXELS(object->x) + gData_02001974[k][0]) >> 4;
      top = (PIXELS(object->z) + gData_02001974[k][1]) >> 4;
      right = (PIXELS(object->x) + gData_02001974[k][2]) >> 4;
      bottom = (PIXELS(object->z) + gData_02001974[k][3]) >> 4;
      if (left > x || x >= right || top > z || z >= bottom)
        continue;
      *kind = k;
      if (k & 1) {
        if (left == leader->x >> 20)
          continue;
      } else {
        if (top == leader->z >> 20)
          continue;
      }
      *slot = i;
      return object;
    }
  }
  return NULL;
}
