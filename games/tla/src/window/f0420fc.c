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
