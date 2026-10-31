// Near miss, differ 4 of 49 (tla-en, 2026-10-31): after the decode the
// native loads the entry mask from the pool before it rereads the count, and
// advances to the entries before the + 1; here sched2 takes the reread first
// (priority 70 to the pool load's 69: the reread's + 1 is one step longer)
// and the advance after the and. Measured: the mask pinned to r2 and set
// first inside a one-pass block holding the count update and the draw,
// differ 2 (that block matches, but its barrier takes the later read of sl
// from mov sl, r1, which then loses its tie with the first count store); the
// pin set before that block, or with only the update in it, 4 to 6; the pin
// with an empty asm, 4; a u16 mask local, 7 to 64; (count)++ then &=, 7;
// the mask written first, 4; permute 4 minutes, nothing better.
// Under -mthumb-slow-loads (2026-10-26): no single empty asm, one-pass
// block, wrapping block or swap of neighbouring statements brings it closer,
// nor any pair of empty asm statements or one-pass blocks (device search).
// Twin of The Broken Seal's Text_DrawMessage.
#include "text_render.h"
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
