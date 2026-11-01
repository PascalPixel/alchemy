// Draft, differ 43 of 148, skeleton gap 9 (tla-en, 2026-11-01)
// Copies a rectangle of cells, keeping each destination's bits from 11 up and
// taking the source's low 11, and redraws the cell picture in each layer whose
// 16 by 12 view shows the destination cell. Family of The Broken Seal's
// MapGrid_CopyCellPictureCellsRect, and closest to its sibling
// MapGrid_CopyCellIndexRect, which keeps the attributes the same way (at bit
// 12) and whose shape this port takes.
// Left: only the cell section at the top of the x loop. The native has the cell
// in r2 and the low bits in r3, so 0xfff goes to ip through r3, is reloaded
// through r5 and the second mask clobbers r3; here local allocation puts the
// cell in r3 (or, with `tile = cell & 0xfff`, the cell in r1 and low tied to
// it). Frame, loops, offset chain and the redraw block all match. Tried without
// gain: declaration order, signedness, a separate mask variable, setting the
// masks before the load, reusing cell for the destination word, compound
// assignments and either operand order. permute could not run (no built
// edition in out/).
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
   * byte offset, offset += 2048), 175 of 148 instructions differ in tla-en. */
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

        tile = cell & 0xfff;
        low = cell & 0x7ff;
        *dst = low | (*dst & 0xfffff800);
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
