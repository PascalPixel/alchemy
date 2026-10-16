// Near miss: differ 15 of 29, registers only. The native keeps the menu work
// in r3, the 96 in r0, the counter in r1 and the pointer in r2; the draft
// allocates r2, r3, r0 and r1. Tried: a walking pointer, an indexed base
// pointer, an asm barrier on the work.
#include "item_icon.h"

void Func_0810bdf4(void) {
  struct IconWork *work = Ram_HeapSlots->menuRuntime;
  s32 i;

  for (i = ICON_COUNT - 1; i >= 0; i--)
    work->resources[i] = ICON_NONE;
  work->active = 0;
  Scheduler_AddOrUpdateCallback(Func_0810bdb0, 0x48a);
}
