#include "types.h"
#include "m7_interfaces.h"
#include "ram_buffer.h"

void PsynergyMenu_RefreshOwnerEntries(s32 originX, s32 originY, s32 columns);

void PsynergyMenu_RefreshOwnerEntries(s32 originX, s32 originY, s32 columns) {
  struct RenderOutput **slot;
  struct RenderOutput **scan;
  s32 index;

  index = 0;
  slot = (struct RenderOutput **)((u8 *)Ram_HeapSlots->menuRuntime + 0x4c);
  scan = slot;
  do {
    if (*scan++ != NULL) {
      PsynergyMenu_PositionOwnerEntry(slot, index, originX, originY, columns);
    }
    index++;
    slot++;
  } while (index <= 0x1F);
}
