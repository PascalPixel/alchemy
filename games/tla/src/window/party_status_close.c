#include "types.h"
#include "ram_buffer.h"
#include "mem.h"
#include "uiflow.h"

void PartyStatus_Close(void) {
  Window_Close(*(void **)Ram_HeapSlots->windowLayout, 1);
  HeapSlot_Release(64);
}
