#include "types.h"
#include "ram_buffer.h"

s32 UiWork_IsCompleteFar(void);
void WaitTicks(s32 ticks);

void Func_08120158(void) {
  while (!UiWork_IsCompleteFar())
    WaitTicks(1);
  ((s32 *)Ram_HeapSlots->unk84[4])[2] = 1;
}
