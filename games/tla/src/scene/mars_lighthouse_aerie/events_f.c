#include "mars_lighthouse_aerie.h"
#include "edition.h"

void Func_02004d14(void) { GameFlag_ClearBit(0x202); }

// Absent from the Japanese edition, as is Func_02004d50.
#if (LANGUAGE != LANGUAGE_JAPANESE)
void Func_02004d24(s32 id) {
  u8 *work = Ram_HeapSlots->windowTiles;

  /* FAKEMATCH: ⚓️ loads the window tiles before it starts the held id's offset
   */
  asm volatile("" : "+l"(work));
  if (((struct MarsLighthouseAerieParty *)&gPartyState)->held == id)
    work[0x138a] = 1;
}
#endif
