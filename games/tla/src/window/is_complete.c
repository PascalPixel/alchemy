/// @file
/// Display: test whether every message channel has finished writing.

#include "types.h"
#include "edition.h"
#include "window.h"
#include "ram_buffer.h"

/// Returns 0 if any of the three channels has a window still writing (state
/// is zero), else 1.
s32 Message_AllTyped(void) {
  struct UiChannelSlot *ch;
  s32 i;

  ch = ((struct WindowWork *)Ram_HeapSlots->windowTiles)->channels;
  for (i = 0; i != 3; i++, ch++) {
    if (ch->work != NULL && ch->work->state == 0)
      return 0;
  }
  return 1;
}
