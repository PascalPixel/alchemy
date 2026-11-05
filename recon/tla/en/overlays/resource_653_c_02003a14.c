// Draft, differ 2 of 30 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Under -Os, 5 November: differ 4 of 30 (tla-en); older figures below predate it.
// Near miss, differ 6 of 30 (tla-en, 2026-10-31): scheduler order only. The
// native loads the address of gData_02006294 first, issues the ldrb of byte
// 0x17 one slot after the store to gData_02006294 (subs r3, #13 between) and
// sets the call argument (movs r0, #0) between the ldrb and the and. The
// sched2 dump shows the ldrb depending on that store (may alias) at cost 1:
// -mthumb-slow-loads skips the one-cycle wait for a load whose address a
// later store in the block writes back, and this bit-field update is such a
// read-modify-write. Worked by hand from the dump's priorities (not
// measured), a cost of 2 gives exactly the native's order: the two address
// loads then tie on priority and the one with more dependents
// (gData_02006294's) goes first. Also measured 2026-10-31: MapWork's u8
// unk017 masked by hand, 6 (movs r3, #243 for the subs); a struct with
// MapWork's s32 fields before the bit-field, 6; the store to gData_02006294
// before the map work load, 13.
// A one-pass block before the bit-field store leaves differ 4. 2026-10-19:
// the call's 0 pinned to r0 before the bit-field store (6, no change).
// Measured (2026-10-26): every spot left is a load the native issues one slot
// after a store to memory it may alias (through a pointer from a call or a
// load, against the stack, the object or a global), with an independent
// instruction between; -mthumb-slow-loads then delayed only a load from the
// address just stored (it now also delays one after a store it may alias,
// except a read-modify-write, as above). A matched pointer
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
  /* FAKEMATCH(barrier): an empty asm holds the next statement after this
   * one; as no asm, 6 of 30 instructions differ in tla-en. */
  asm("");
  /* FAKEMATCH(barrier): an empty asm holds the order around it; as no asm,
   * 4 of 30 instructions differ in tla-en. */
  do {
    map->mode = 1;
    Func_02005338(0);
  } while (0);
  if (gMainRoutineWork.entrance == 1 || gMainRoutineWork.entrance == 2)
    GameFlag_ClearBit(0x12f);
}
