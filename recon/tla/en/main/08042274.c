// Near miss: its instructions match under -mthumb-slow-loads (differ 0 of
// 74, 2026-10-25), but its literal pool does not: the u16 temporary is
// promoted, so 0xf01e is pooled as the word 0xfffff01e, where the native
// holds 0x0000f01e (a halfword pool entry and its two bytes of padding, as
// consttable_2 writes one). Adopted in en and de it compared 2 bytes short
// of the ROM there. Without the temporary the constant is built in three
// instructions (differ 6); as s16, differ 2 with the same pool word.
// The Broken Seal's Text_DrawPrefixedNumberAtOffset in the form of its
// English edition (its Spanish, French and Italian ones set both halfwords
// in the branches; this game's Japanese edition has another layout),
// reading the window's x and y as signed halfwords, with a barrier after
// the second halfword's store (without it, differ 2). Before the new
// option the native loaded the fifth argument from the stack and the window
// tiles before it copied the value (differ 2).
#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "render_input.h"
#include "text_render.h"

u8 *String_FormatNumber(u8 *output, s32 value, s32 width);

void Text_DrawPrefixedNumberAtOffset(s32 value, struct RenderInput *work,
                                       s32 offsetX, s32 offsetY, s32 variant) {
  u8 formatted[16];
  u16 output[8];
  u8 *text;
  s32 index;
  u32 cell;
  struct WindowWork *canvas;
  s32 vram;
  s32 src;
  s32 dst;
  s32 phase;
  u16 second;

  canvas = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  text = String_FormatNumber(formatted, value, 4);
  if (variant == 0) {
    output[0] = 0xf01d;
  } else {
    output[0] = 0xf01f;
  }
  second = 0xf01e;
  output[1] = second;
  asm("");
  for (index = 0; index <= 4; index++) {
    output[index + 2] = text[index];
  }
  output[6] = 0;
  cell = (((s16)work->y + ((u32)offsetY >> 3) + 1) << 5) +
         ((s16)work->x + ((u32)offsetX >> 3)) + 1;
  if (cell < 0x280) {
    vram = 0x06002000;
    cell *= 2;
    dst = vram + cell;
    src = (s32)canvas->tilemap + cell;
    phase = offsetX & 7;
    Text_DrawStringAsTiles((u16 *)output, (u16 *)src, (u16 *)dst, phase);
  }
}
