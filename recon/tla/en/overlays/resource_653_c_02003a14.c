// Near miss, differ 6 of 30, order only: the native loads the address of
// gData_02006294 first and sets the call argument (movs r0, #0) between the
// mask and the and; a one-pass block before the bit-field store leaves
// differ 4. 2026-10-19: the call's 0 pinned to r0 before the bit-field store
// (6, no change).
// Measured (2026-10-26): every spot left is a load the native issues one slot
// after a store to memory it may alias (through a pointer from a call or a
// load, against the stack, the object or a global), with an independent
// instruction between; -mthumb-slow-loads delays only a load from the address
// just stored, so this draft issues the load at once. A matched pointer
// parameter (Gabomba Catacombs' Func_020008a0) cannot alias the stack and
// shows no gap, which fits. No source form found to make it wait.
#include "../../../../games/tla/src/scene/kandorean_temple/local.h"

struct MapWork17 {
  u8 unk00[0x17];
  u8 bits0 : 2;
  u8 mode : 2;
};

void Func_02003a14(void) {
  struct MapWork17 *map = (struct MapWork17 *)Ram_HeapSlots->mapWork;

  gData_02006294 = 0;
  map->mode = 1;
  Func_02005338(0);
  if (gAdventure.entrance == 1 || gAdventure.entrance == 2)
    GameFlag_ClearBit(0x12f);
}
