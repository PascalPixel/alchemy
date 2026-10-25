// Draft, differ 2 of 22 (2026-10-25; was 4): the count read into a local
// before an empty asm brings its ldrh up beside the native's. Left: the native
// loads the ring mask (ldr r1, 0x1ff) right after movs r4, #255, before the
// 0xffff is finished; here it comes two rows later. Measured: the mask in a
// local before or after the asm (differ 19), the decrement as + 0xffff
// (5), a u16 or s32 count (4, 9), a second asm between the two steps (7).
#include "types.h"
#include "window.h"
#include "ram_buffer.h"

s32 UiText_BuildRenderEntries(s32 message, s32 mode);

void Func_0803cf6c(s32 message) {
  struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
  u32 count = work->count;

  /* FAKEMATCH(barrier): an empty asm, so the count is read before the
   * 0xffff is built; as plain statements, 4 of 22 instructions differ in
   * tla-en. */
  __asm__ volatile("");
  count = (u16)(count - 1) & (UI_TEXT_ENTRY_COUNT - 1);
  work->count = (u16)(count - 1) & (UI_TEXT_ENTRY_COUNT - 1);
  UiText_BuildRenderEntries(message, 1);
}
