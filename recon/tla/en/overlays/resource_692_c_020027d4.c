// Draft, differ 28 of 62: checks the leader's cell on layer 2: off cell values 1
// and 7 it stamps the cells (Func_0200257c), plays the glow at column 264 in
// the row for the event's current number (event work byte 362, from 201), sets
// flag 0x200 and that number's flag (312 on); on them it replays the glow only
// once flag 0x200 is set.
#include "../../../../games/tla/src/scene/magma_rock_inside/local.h"

s32 Func_02000658(s32 layer, s32 x, s32 z);
void Func_0200257c(void);
void Func_020026a4(s32 x, s32 z);

void Func_020027d4(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 value = Func_02000658(2, leader->x >> 20, leader->z >> 20) >> 8;
  s16 *number = (s16 *)(work + 362);
  s32 row = 54 - ((*number - 201) << 2);

  if (value != 1 && value != 7) {
    Func_0200257c();
    Func_020026a4(264, (row << 3) + 10);
    if (!GameFlag_Test(0x200))
      GameFlag_SetBit(0x200);
    GameFlag_SetBit(*number + 312);
  } else if (GameFlag_Test(0x200)) {
    Func_020026a4(264, (row << 3) + 10);
  }
}
