// Near miss, differ 6 of 62 (tla-en, 2026-10-31): after the terminator store
// the native shifts the row offset (lsrs r2, r4, #3) before it loads the
// window row (ldrsh), and copies the column offset before adding them; this
// loads the row first. The sched2 dump shows why: the row load is ready one
// cycle after the store (-mthumb-slow-loads delays only whole loads after a
// store, not a sign- or zero-extended one) and outranks the shift, priority
// 10 to 8, so no statement order moves it. The loop's empty asm stands in for
// the same wait before its ldrb. No single empty asm, one-pass block,
// wrapping block or swap of neighbouring statements brings it closer, nor any
// pair of empty asm statements or one-pass blocks (device search, 2026-10-26).
// Measured: the shift written first in the sum is differ 12; separate row and
// column statements or a (s16 *) read stay at 6; the shift as a temporary
// behind an empty asm barrier changes the high-register copies (differ 29);
// four minutes of permuter found nothing better. Twin of The Broken Seal's
// Text_DrawStringAtOffset with this game's loop barrier.
#include "heap_release.h"
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
    /* FAKEMATCH(barrier): the empty asm kept; as the plain loop, 8 of 62
       instructions differ in tla-en. */
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
