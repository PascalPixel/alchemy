#include "types.h"
#include "tla_edition.h"
#include "ram_buffer.h"

void Window_SetBusyFlags(u32 flags) {
  u8 *work;

  work = Ram_HeapSlots->windowTiles;
  if (work == 0)
    return;
  if (flags & 1)
    work[WINDOW_BUSY_FLAGS_OFFSET] = 1;
  if (flags & 2)
    work[WINDOW_BUSY_FLAGS_OFFSET + 1] = 1;
}
