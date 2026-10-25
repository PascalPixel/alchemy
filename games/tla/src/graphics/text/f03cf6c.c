#include "types.h"
#include "window.h"
#include "ram_buffer.h"

s32 UiText_BuildRenderEntries(s32 message, s32 mode);

void Func_0803cf6c(s32 message) {
  struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
  u32 count = work->count;

  count = (u16)(count - 1) & (UI_TEXT_ENTRY_COUNT - 1);
  work->count = (u16)(count - 1) & (UI_TEXT_ENTRY_COUNT - 1);
  UiText_BuildRenderEntries(message, 1);
}
