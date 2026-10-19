#include "types.h"
#include "ram_buffer.h"
#include "mem.h"
#include "uiflow.h"

void PartyStatus_Close(void) {
  UiWork_Finalize(*(void **)Ram_HeapSlots->windowLayout, 1);
  Runtime_ReleaseHeapBlock(64);
}
