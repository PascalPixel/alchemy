// Draft, differ 159 of 132: finds the pushable object in front of the leader:
// records the leader's direction (its angle's high four bits), then for
// objects 8 to 63 (the event work's object table) and the six kinds of
// gData_02004ca4 tests the point one step ahead (gData_02004cc8, packed
// offsets by direction) against the kind's box (gData_02004cb0); a hit not
// already level with the leader on the kind's axis records the kind and the
// object's number and is returned, else 0. The same listing as overlay
// 674's Func_02003940 and 662's Func_0200033c. Left: the native keeps the
// three out pointers on the stack, the object number in r11 and the table
// cursor in lr, and builds the box edges in r8 to r12; the alignment is
// poor.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"

extern u16 gData_02004ca4[];
extern s8 gData_02004cb0[][4];
extern s32 gData_02004cc8[];
struct ObjectRuntime *Object_GetById(u32 id);

struct ObjectRuntime *Func_020043a8(s32 *direction, s32 *number, s32 *kind) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  struct ObjectRuntime **slot = (struct ObjectRuntime **)(work + 0x34);
  struct ObjectRuntime *object;
  u32 n, k;
  s32 aheadX, aheadZ, left, top, right, bottom;

  *direction = leader->angle >> 12;
  for (n = 8; n <= 63; n++, slot++) {
    object = *slot;
    for (k = 0; k <= 5; k++) {
      if (*((struct BattleAnimationState *)object->animation)->value28 !=
          gData_02004ca4[k])
        continue;
      aheadX = (((s16 *)&leader->x)[1] + (gData_02004cc8[*direction] >> 16)) >>
               4;
      aheadZ = (((s16 *)&leader->z)[1] + (s16)gData_02004cc8[*direction]) >> 4;
      left = (((s16 *)&object->x)[1] + gData_02004cb0[k][0]) >> 4;
      top = (((s16 *)&object->z)[1] + gData_02004cb0[k][1]) >> 4;
      right = (((s16 *)&object->x)[1] + gData_02004cb0[k][2]) >> 4;
      bottom = (((s16 *)&object->z)[1] + gData_02004cb0[k][3]) >> 4;
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
