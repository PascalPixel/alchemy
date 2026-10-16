// Near miss, differ 2 of 49: the listing copies the mask 0x40 to r3 one
// instruction later (after it loads the owner's x). sched2 ranks the load of
// the owner's x (priority 107) below the mask's copy (108). Also tried: a mask
// local, the operands swapped, the shift of x in its own statement, the masked
// x in an s16 local, a barrier between x and z (2 each); the cell and owner
// reads ahead of a barrier (7); four minutes of permute (nothing better).
#include "../../../../games/tla/src/scene/jupiter_room_7/jupiter_room_7.h"

void Func_02002534(struct JupiterRoom7Cell *cell) {
  struct ObjectRuntime *owner = Object_GetById(gPartyState.currentOwner);
  s32 x = (owner->x >> 20) | (s16)(cell->x & 0x40);
  s32 z = (owner->z >> 20) | (s16)(cell->z & 0x40);

  Func_020024ac();
  Func_02002c5c(161);
  Func_02002b1c(cell->x, cell->z + 1, 1, 1, x, z);
  Battle_WaitMode0(12);
}
