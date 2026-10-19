#include "types.h"
#include "ram_buffer.h"

/// When the event work's mode byte at 0x18a is 3, copies it to the map
/// work's byte at 0x970.
void Func_080dc954(void) {
  struct HeapSlots *slots = Ram_HeapSlots;
  s32 mode = *(s8 *)((u8 *)slots->eventWork + 0x18a);

  if (mode == 3)
    *((u8 *)slots->mapWork + 0x970) = mode;
}
