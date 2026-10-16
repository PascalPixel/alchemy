#include "mars_aerie.h"

void Func_02004d14(void) { GameFlag_ClearBit(0x202); }

// Absent from the Japanese edition, as is Func_02004d50.
#if !defined(TLA_EDITION_JA)
void Func_02004d24(s32 id) {
  u8 *work = Ram_HeapSlots->windowTiles;

  /* FAKEMATCH: ⚓️ loads the window tiles before it starts the held id's offset
   */
  asm volatile("" : "+l"(work));
  if (((struct MarsAerieParty *)&gPartyState)->held == id)
    work[0x138a] = 1;
}
#endif
