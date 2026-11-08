#include "mars_lighthouse_aerie.h"
#include "version.h"

void Func_02004d14(void) { GameFlag_ClearBit(0x202); }

// From patch 1, as is Func_02004d50.
#if TLA_PATCH_1
void Func_02004d24(s32 id) {
  u8 *work = Ram_HeapSlots->windowTiles;

  if (((struct MarsLighthouseAerieParty *)&gMainRoutineWork)->held == id)
    work[0x138a] = 1;
}
#endif
