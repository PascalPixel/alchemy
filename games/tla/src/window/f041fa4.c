#include "text_render.h"
#include "window.h"
#include "ram_buffer.h"

s32 Message_Decode(s32 message, s32 mode);
void Text_DrawWideStringAtOffset(u16 *text, struct Window *window, s32 x,
                                 s32 y);

void Text_DrawMessage(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
  struct WindowWork *canvas = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u16 *counter = &canvas->count;
  s32 zero = 0;

  *counter = zero;
  Message_Decode(arg0, 1);
  /* FAKEMATCH(one-pass): the closing 0's store in a one-pass block, whose
   * end keeps the count's reread after the mask's load; as a plain
   * statement, 4 of 49 instructions differ in tla-en. */
  do {
    canvas->entries[*counter] = zero;
  } while (0);
  {
    /* FAKEMATCH(pin): the mask pinned to the r2 it takes, so it loads from
     * the pool before the count is reread; as a plain constant, 4 of 49
     * instructions differ in tla-en. */
    register u16 mask asm("r2") = UI_TEXT_ENTRY_COUNT - 1;

    *counter = (*counter + 1) & mask;
  }
  Text_DrawWideStringAtOffset(canvas->entries, (struct Window *)arg1, arg2,
                              arg3);
}
