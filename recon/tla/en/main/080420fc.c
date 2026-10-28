// Draft, differ 6 of 62 in tla-en under -mthumb-slow-loads (2026-10-26). No
// single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer, nor any pair of empty asm statements or
// one-pass blocks (device search).
// Draft, differ 4 of 62: after the terminator store the native shifts the row
// offset (lsrs r2, r4, #3) before it loads the window row, and adds them
// after copying the column offset; this loads the row first. Measured: the
// shift written first in the sum is differ 10; the shift as a temporary
// behind an empty asm barrier changes the high-register copies (differ 29);
// four minutes of permuter found nothing better. Twin of The Broken Seal's
// Text_DrawStringAtOffset with this game's loop barrier.
#include "mem.h"
#include "text_render.h"
#include "window.h"
#include "ram_buffer.h"

void Text_DrawStringAtOffset(u8 *text, struct RenderInput *work, s32 offsetX,
                               s32 offsetY) {
  u16 *buffer;
  u16 *output;
  struct WindowWork *canvas;
  u32 cell;
  s32 vram;
  s32 src;
  s32 dst;
  s32 phase;

  buffer = Heap_AllocEwram(0x200);
  canvas = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  output = buffer;
  while (*text != 0) {
    *output = *text;
    text++;
    output++;
    /* FAKEMATCH: ⚓️ steps the output before it reads the next byte. */
    __asm__("");
  }
  *output = 0;

  cell = (((s16)work->y + ((u32)offsetY >> 3) + 1) << 5) +
         ((s16)work->x + ((u32)offsetX >> 3)) + 1;
  if (cell < 0x280) {
    vram = 0x06002000;
    cell *= 2;
    dst = vram + cell;
    src = (s32)canvas->tilemap + cell;
    phase = offsetX & 7;
    Text_DrawStringAsTiles(buffer, (u16 *)src, (u16 *)dst, phase);
    Heap_Release(buffer);
  }
}
