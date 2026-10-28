#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "heap_release.h"
#include "text_render.h"

u8 *String_FormatNumber(u8 *output, s32 value, s32 width);

void Text_DrawStringInWindow(u8 *text, s32 arg1, u32 x, u32 y) {
  u16 *buffer = Heap_AllocEwram(0x200);
  u16 *output = buffer;

  while (*text != 0) {
    *output = *text;
    text++;
    output++;
    /* FAKEMATCH: ⚓️ steps the output before it reads the next byte. */
    __asm__("");
  }
  x >>= 3;
  *output = 0;
  y >>= 3;
  Text_RenderWideStringInWindow(buffer, arg1, x, y);
  Heap_Release(buffer);
}

/// The number writers format the value in a 16-byte buffer and draw it with
/// the matching string writer.
void Text_DrawNumber(s32 value, s32 width, s32 output, s32 x, s32 y) {
  u8 data[16];
  /* FAKEMATCH: the value is pinned to the r4 it takes, so the fifth argument
   * loads from the stack before the first is copied; as the plain parameter,
   * 2 instructions differ in tla-en. */
  register s32 pinned asm("r4") = value;

  Text_DrawString(String_FormatNumber(data, pinned, width), output, x, y);
}

void Text_DrawNumberAtOffset(s32 value, s32 width, struct RenderInput *work,
                               s32 x, s32 y) {
  u8 data[16];
  /* FAKEMATCH: pinned to r4 as in Text_DrawNumber (differ 2 without). */
  register s32 pinned asm("r4") = value;

  Text_DrawStringAtOffset(String_FormatNumber(data, pinned, width), work, x,
                            y);
}

void Text_DrawNumberInWindow(s32 value, s32 width, s32 output, s32 x, s32 y) {
  u8 data[16];
  /* FAKEMATCH: pinned to r4 as in Text_DrawNumber (differ 2 without). */
  register s32 pinned asm("r4") = value;

  Text_DrawStringInWindow(String_FormatNumber(data, pinned, width), output, x,
                            y);
}
