#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"

/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

extern u16 gData_020022d0[];
extern s8 gData_020022dc[][4];
extern s32 gData_020022f4[];

struct Actor *Actor_GetById(s32 id);

struct Actor *Func_02000dac(s32 *direction, s32 *slot, s32 *kind) {
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
      gData_020022d0[k]) {
    x = (PIXELS(leader->x) + (gData_020022f4[*direction] >> 16)) >> 4;
    z = (PIXELS(leader->z) + (s16)gData_020022f4[*direction]) >> 4;
    left = (PIXELS(object->x) + gData_020022dc[k][0]) >> 4;
    top = (PIXELS(object->z) + gData_020022dc[k][1]) >> 4;
    right = (PIXELS(object->x) + gData_020022dc[k][2]) >> 4;
    bottom = (PIXELS(object->z) + gData_020022dc[k][3]) >> 4;
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
  entry++;
  i++;
  if (i <= 63)
    goto outer;
  return NULL;
}
