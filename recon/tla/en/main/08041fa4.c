// Draft, differ 4 of 49 in tla-en under -mthumb-slow-loads (2026-10-26). No
// single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer, nor any pair of empty asm statements or
// one-pass blocks (device search).
// Draft, differ 10 of 49, three pairs in the other order: the native loads
// the window work before it starts the count's offset, loads the entry mask
// from the pool before it rereads the count, and advances to the entries
// before the mask is applied. Measured: an empty asm barrier after the
// window work swaps r5 and r6 (differ 22). Twin of The Broken Seal's
// Text_DrawMessage.
#include "text_render_runtime.h"
#include "window.h"
#include "ram_buffer.h"

s32 Message_Decode(s32 message, s32 mode);
void Text_DrawWideStringAtOffset(u16 *text, struct Window *window, s32 x,
                                     s32 y);

void Text_DrawMessage(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
  struct WindowWork *canvas =
      (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u16 *counter = &canvas->count;
  s32 zero = 0;

  *counter = zero;
  Message_Decode(arg0, 1);
  canvas->entries[*counter] = zero;
  *counter = (*counter + 1) & (UI_TEXT_ENTRY_COUNT - 1);
  Text_DrawWideStringAtOffset(canvas->entries, (struct Window *)arg1,
                                  arg2, arg3);
}
