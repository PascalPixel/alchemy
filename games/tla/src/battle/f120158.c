#include "types.h"
#include "ram_buffer.h"

s32 Message_AllTypedFar(void);
void WaitFrames(s32 frames);

void Func_08120158(void) {
  while (!Message_AllTypedFar())
    WaitFrames(1);
  ((s32 *)Ram_HeapSlots->unk84[4])[2] = 1;
}
