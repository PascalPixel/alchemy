// Near miss, differ 181 of 148 (2026-10-18), first pass: copies a rectangle
// of map cells (keeping each destination's attribute bits above 11, taking
// the source's low 11) and redraws the block in each layer whose 16 by 12
// view shows the destination cell, like The Broken Seal's
// Map_CopyMetatileIndicesRect. Left: the native's stack frame is 36 bytes (row end,
// the two cell cursors, the three views) with width in r11; this spills
// width too (44 bytes) and allocates the view loop differently.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapCellBuffer[];

void Func_0802b450(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                   s32 dstY) {
  u32 *src = (u32 *)gMapCellBuffer + (srcY << 7) + srcX;
  u32 *dst = (u32 *)gMapCellBuffer + (dstY << 7) + dstX;
  s32 views[3][2];
  struct MapLayer *layer = Ram_HeapSlots->mapWork->layers;
  s32 *view = views[0];
  s32 i;
  s32 row;
  s32 col;

  for (i = 2; i >= 0; i--) {
    view[0] = layer->x >> 20;
    view[1] = layer->y >> 20;
    layer++;
    view += 2;
  }
  for (row = dstY; row < dstY + height; row++) {
    for (col = dstX; col < dstX + width; col++) {
      u32 cell = *src++;
      u32 tile = cell & 0xfff;
      u32 offset;

      *dst = (*dst & 0xfffff800) | (cell & 0x7ff);
      dst++;
      offset = (((row & 15) << 5) + (col & 15)) * 4;
      view = views[0];
      for (i = 0; i <= 2; i++) {
        if (view[0] <= col && view[0] + 16 > col && view[1] <= row &&
            view[1] + 12 > row) {
          *(u32 *)(offset + 0x06002800) = *(u32 *)(tile * 8 + 0x02020000);
          *(u32 *)(offset + 0x06002840) = *(u32 *)(tile * 8 + 0x02020004);
          break;
        }
        offset += 0x800;
        view += 2;
      }
    }
    src += 128 - width;
    dst += 128 - width;
  }
}
