// Draft, differ 159 of 132: overlay 674's copy of overlay 668's
// Func_020043a8 (the same listing), whose draft holds its notes.
#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "ram_buffer.h"

extern u16 gData_02004070[];
extern s8 gData_0200407c[][4];
extern s32 gData_02004094[];
struct Actor *Actor_GetById(u32 id);

struct Actor *Func_02003940(s32 *direction, s32 *number, s32 *kind) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_GetById(gAdventure.currentOwner);
  struct Actor **slot = (struct Actor **)(work + 0x34);
  struct Actor *object;
  u32 n, k;
  s32 aheadX, aheadZ, left, top, right, bottom;

  *direction = leader->angle >> 12;
  for (n = 8; n <= 63; n++, slot++) {
    object = *slot;
    for (k = 0; k <= 5; k++) {
      if (*((struct ObjectAnimationState *)object->animation)->value28 !=
          gData_02004070[k])
        continue;
      aheadX = (((s16 *)&leader->x)[1] + (gData_02004094[*direction] >> 16)) >>
               4;
      aheadZ = (((s16 *)&leader->z)[1] + (s16)gData_02004094[*direction]) >> 4;
      left = (((s16 *)&object->x)[1] + gData_0200407c[k][0]) >> 4;
      top = (((s16 *)&object->z)[1] + gData_0200407c[k][1]) >> 4;
      right = (((s16 *)&object->x)[1] + gData_0200407c[k][2]) >> 4;
      bottom = (((s16 *)&object->z)[1] + gData_0200407c[k][3]) >> 4;
      if (left > aheadX || aheadX >= right || top > aheadZ ||
          aheadZ >= bottom)
        continue;
      *kind = k;
      if (k & 1) {
        if (left == leader->x >> 20)
          continue;
      } else {
        if (top == leader->z >> 20)
          continue;
      }
      *number = n;
      return object;
    }
  }
  return 0;
}
