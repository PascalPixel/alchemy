// Misfiled draft (tla-en, 2026-10-18): the listing at 0x0803b094 is
// Message_Decode (831 instructions), which this file only
// declares. It holds the text writers after it instead: Text_DrawMessage,
// Text_DrawString (since adopted, games/tla/src/graphics/text/draw_string.c),
// Text_DrawStringAtOffset, Text_DrawStringInWindow and the number writers
// (adopted, games/tla/src/graphics/text/render.c), and
// Text_DrawPrefixedNumberAtOffset (listing 0x08042274). alchemy-devtools diff
// against the listing reports differ 1071, which measures nothing.

#include "mem.h"
#include "text_render_runtime.h"
#include "ram_buffer.h"
#include "window.h"

s32 Message_Decode(s32 character, s32 count);
u8 *Text_FormatNumber(u8 *output, s32 value, s32 width);
void Text_DrawWideString(void *text, s32 work, s32 x, s32 y);
void Text_DrawWideStringInWindow(u16 *text, s32 work, s32 x, s32 y);
s32 Text_DrawTileRow(void *text, s32 source, s32 destination, s32 phase);

void Text_DrawMessage(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
  u8 *base = (u8 *)Ram_HeapSlots->windowTiles;
  u16 *counter = (u16 *)(base + offsetof(struct WindowWork, count));
  s32 offset;
  s32 zero = 0;

  *counter = zero;
  Message_Decode(arg0, 1);
  offset = *counter * 2 + offsetof(struct WindowWork, entries);
  *(u16 *)(base + offset) = zero;
  *counter = (*counter + 1) & (UI_TEXT_ENTRY_COUNT - 1);
  Text_DrawWideString(base + offsetof(struct WindowWork, entries), arg1, arg2, arg3);
}

void Text_DrawString(u8 *text, s32 arg1, s32 arg2, s32 arg3) {
  s16 *buffer;
  s16 *output;
  u8 *input;

  input = text;
  buffer = Runtime_BumpAllocateAlternatePool(0x200);
  output = buffer;
  if (*input != 0) {
    do {
      *output = (s16)*input;
      input++;
      output++;
    } while (*input != 0);
  }
  *output = 0;
  Text_DrawWideString(buffer, arg1, arg2, arg3);
  Runtime_ReleaseAlternatePool(buffer);
}

void Text_DrawStringAtOffset(u8 *text, struct RenderInput *work, s32 offsetX,
                               s32 offsetY) {
  s16 *buffer;
  s16 *output;
  u8 *base;
  u32 cell;
  s32 vram;
  s32 src;
  s32 dst;
  s32 phase;

  buffer = Runtime_BumpAllocateAlternatePool(0x200);
  base = (u8 *)Ram_HeapSlots->windowTiles;
  output = buffer;
  if (*text != 0) {
    do {
      *output = *text;
      text++;
      output++;
    } while (*text != 0);
  }
  *output = 0;

  cell = ((work->y + ((u32)offsetY >> 3) + 1) << 5) +
         (work->x + ((u32)offsetX >> 3)) + 1;
  if (cell < 0x280) {
    vram = 0x06002000;
    cell *= 2;
    dst = vram + cell;
    src = (s32)base + cell + 8;
    phase = offsetX & 7;
    Text_DrawTileRow(buffer, src, dst, phase);
    Runtime_ReleaseAlternatePool(buffer);
  }
}

void Text_DrawStringInWindow(u8 *text, s32 arg1, u32 x, u32 y) {
  u16 *buffer = Runtime_BumpAllocateAlternatePool(0x200);
  u16 *output = buffer;

  while (*text != 0) {
    *output = *text;
    text++;
    output++;
  }
  *output = 0;
  x >>= 3;
  y >>= 3;
  Text_DrawWideStringInWindow(buffer, arg1, x, y);
  Runtime_ReleaseAlternatePool(buffer);
}

void Text_DrawNumber(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
  u8 data[16];

  Text_DrawString(Text_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}

void Text_DrawNumberAtOffset(s32 arg0, s32 arg1, struct RenderInput *arg2,
                               s32 arg3, s32 arg4) {
  u8 data[16];

  Text_DrawStringAtOffset(Text_FormatNumber(data, arg0, arg1), arg2, arg3,
                            arg4);
}

void Text_DrawNumberInWindow(s32 arg0, s32 arg1, s32 arg2, s32 arg3,
                               s32 arg4) {
  u8 data[16];

  Text_DrawStringInWindow(Text_FormatNumber(data, arg0, arg1), arg2, arg3,
                            arg4);
}

void Text_DrawPrefixedNumberAtOffset(s32 value, struct RenderInput *work,
                                       s32 offsetX, s32 offsetY, s32 variant) {
  u8 formatted[16];
  s16 output[8];
  u8 *text;
  s32 index;
  u32 cell;
  u8 *base;
  s32 vram;
  s32 src;
  s32 dst;
  s32 phase;

  base = (u8 *)Ram_HeapSlots->windowTiles;
  text = Text_FormatNumber(formatted, value, 4);
  if (variant == 0) {
    output[0] = 0xf01d;
  } else {
    output[0] = 0xf01f;
  }
  output[1] = 0xf01e;
  for (index = 0; index <= 4; index++) {
    output[index + 2] = text[index];
  }
  output[6] = 0;

  cell = ((work->y + ((u32)offsetY >> 3) + 1) << 5) +
         (work->x + ((u32)offsetX >> 3)) + 1;
  if (cell < 0x280) {
    vram = 0x06002000;
    cell *= 2;
    dst = vram + cell;
    src = (s32)base + cell + 8;
    phase = offsetX & 7;
    Text_DrawTileRow(output, src, dst, phase);
  }
}
