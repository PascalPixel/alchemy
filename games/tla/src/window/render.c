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
  s32 pinned = value;

  Text_DrawString(String_FormatNumber(data, pinned, width), output, x, y);
}

void Text_DrawNumberAtOffset(s32 value, s32 width, struct RenderInput *work,
                               s32 x, s32 y) {
  u8 data[16];
  s32 pinned = value;

  Text_DrawStringAtOffset(String_FormatNumber(data, pinned, width), work, x,
                            y);
}

void Text_DrawNumberInWindow(s32 value, s32 width, s32 output, s32 x, s32 y) {
  u8 data[16];
  s32 pinned = value;

  Text_DrawStringInWindow(String_FormatNumber(data, pinned, width), output, x,
                            y);
}
