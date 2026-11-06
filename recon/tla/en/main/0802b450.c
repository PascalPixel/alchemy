// Differ 32 of 148 (tla-en, 2026-11-06).
// Near miss, allocation only: the cell section at the top of the x loop. The
// native has the cell in r2 and the kept low bits in r3, tied to the 0x7ff
// mask; here regmove ties the low bits to the cell (r1), as CSE always puts
// the cell first in the and, whichever order or declaration order the source
// gives. The tile mask (0xfff into ip, then and through r5)
// and the 3 instructions it lacked now match: tile = 0xfff, tile = cell & tile,
// with the 0x7ff load between them so reload cannot reuse r3. A 0x7ff mask set
// once is hoisted out of the x loop (83 differ), so mask also takes the
// destination word.
// Copies a rectangle of cells, keeping each destination's bits from 11 up and
// taking the source's low 11, and redraws the cell picture in each layer whose
// 16 by 12 view shows the destination cell. Family of The Broken Seal's
// MapGrid_CopyCellPictureCellsRect, and closest to its sibling
// MapGrid_CopyCellIndexRect, which keeps the attributes the same way (at bit
// 12) and whose shape this port takes.
// Earlier (2026-11-01, 43 differ): the native has the cell in r2 and the low
// bits in r3, so 0xfff goes to ip through r3, is reloaded through r5 and the
// second mask clobbers r3; local allocation had put the cell in r3 (or, with
// `tile = cell & 0xfff`, the cell in r1 and low tied to it). Frame, loops,
// offset chain and the redraw block all match. Tried without gain:
// declaration order, signedness, a separate mask variable, setting the masks
// before the load, reusing cell for the destination word, compound assignments
// and either operand order. permute (5 minutes,
// 2026-11-06) found only a one-pass block at 34 differ; an 8-minute run
// (2026-11-06, later) nothing below 34.
// Later 2026-11-06: in the regmove dump the and keeps the cell's pseudo
// (`cell = cell & mask`) and the or keeps the destination word's; the native
// keeps the 0x7ff pseudo for the and and the low bits for the or. Making the
// 0x7ff pseudo the result (`low = 0x7ff; low &= cell` or `mask &= cell`) ties
// the and correctly but the 0xfff then goes through r2 straight into ip and
// the cell into r3 (51); also tried: the or as `low | (...)` (42), `mask &
// cell` (32, no change), the tile and after the low one (55), the cell loaded
// after the masks are set (104, 110).
#include "types.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapCellBuffer[];

struct TilePos {
  s32 x, y;
};

void Func_0802b450(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                   s32 dstY) {
  /* FAKEMATCH(temporary): from the twin, scaled holds the cell's offset and
   * then the picture's, n its index and then the layer step, and the addresses
   * are set in parts; as a plain body (the offset computed once, a block-local
   * byte offset, offset += 2048), 175 of 148 instructions differ in tla-en.
   * mask holds 0x7ff and then the destination word, and tile is set to 0xfff
   * before its and; as `tile = cell & 0xfff; low = cell & 0x7ff;
   * low |= *dst & 0xfffff800; *dst = low;`, 35 of 148 differ. */
  s32 y;
  u32 *src = (u32 *)gMapCellBuffer + (srcY * 128 + srcX);
  u32 *dst = &((u32 *)gMapCellBuffer)[dstY * 128 + dstX];
  struct MapLayer *layer = Ram_HeapSlots->mapWork->layers;
  struct TilePos views[3];
  s32 i;
  struct TilePos *view = views;
  s32 x;

  i = 2;
  while (i >= 0) {
    s32 position;

    position = layer->x >> 20;
    view->x = position;
    position = (*layer).y;
    view->y = position >> 20;
    view++;
    layer++;
    i--;
  }
  y = dstY;
  if (y < dstY + height) {
    do {
      for (x = dstX; x < dstX + width; x++) {
        u32 cell = *src++;
        u32 tile;
        u32 offset;
        u32 row;
        u32 low;
        u32 n;
        u32 column;
        u32 scaled;
        u32 mask;

        tile = 0xfff;
        mask = 0x7ff;
        tile = cell & tile;
        low = cell & mask;
        mask = (*dst & 0xfffff800) | low;
        *dst = mask;
        dst++;
        column = x & 15;
        i = 0;
        row = y & 15;
        n = row << 5;
        view = views;
        n += column;
        scaled = n * 4;
        offset = scaled;
        for (; i < 3; i += 1) {
          if (view->x <= x && 16 + (*view).x > x && view->y <= y &&
              view->y + 12 > y) {
            u32 screenAddress, pictureAddress;
            u8 *screenTile, *pictureRows;

            screenAddress = 0x06002800;
            screenTile = (u8 *)screenAddress;
            screenTile += offset;
            scaled = tile * 8;
            pictureAddress = 0x02020000;
            pictureRows = (u8 *)pictureAddress;
            pictureRows += scaled;
            *(u32 *)screenTile = *(u32 *)pictureRows;

            pictureAddress = 0x02020004;
            pictureRows = (u8 *)pictureAddress;
            pictureRows += scaled;
            screenAddress = 0x06002840;
            screenTile = (u8 *)screenAddress;
            screenTile += offset;
            *(u32 *)screenTile = *(u32 *)pictureRows;
            break;
          }
          n = 2048;
          offset += n;
          view++;
        }
      }
      src = src + (128 - width);
      dst = 128 - width + dst;
      y++;
    } while (y < dstY + height);
  }
}
