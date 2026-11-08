// Matches: differ 0 (tla-en, 2026-11-08)
// Every edition measures differ 0 (tla-ja, tla-de, tla-es, tla-fr, tla-it,
// 2026-11-08); the 320 bytes are the same in all six (Japanese at 0802b3cc).
// Copies a rectangle of cells, keeping each destination's bits from 11 up and
// taking the source's low 11, and redraws the cell picture in each layer whose
// 16 by 12 view shows the destination cell. Family of The Broken Seal's
// MapGrid_CopyCellPictureCellsRect, and closest to its sibling
// MapGrid_CopyCellIndexRect, which keeps the attributes the same way (at bit
// 12) and whose shape this port takes.
// How it matched (2026-11-08): the cell section is local allocation. The
// native keeps the 0x7ff mask in r3 through the and, the or and the store,
// with the destination word in r2 and 0xfffff800 in r1; local-alloc gives r3
// to the quantity with the highest refs * log2(refs) / life, ties going to
// the earlier-born one. The word and 0xfffff800 both score 6.0; the mask
// (0x7ff set, and, or, store: 18 refs over 14) only 5.14, so the word took r3.
// Setting 0x7ff after the tile and (life 12, 6.0, wins the tie) gives the
// native's registers but lets reload inherit the 0xfff in r3 for the tile
// and (35): the native builds 0x7ff between `tile = 0xfff` and the and, so
// the and goes ip -> r5 -> ip. The empty asm on mask keeps 0x7ff there and
// adds the two references (24 over 16, 6.0). Moving `view = views` first puts
// view's r1 live across `column = x & 15`, so its reload of the 15 takes r3
// and sched2 orders `mov r1, r9` before `movs r5, #0` as the native does (14
// before the move). A milder device was not found: copies of mask (`low =
// mask`) are folded by CSE, combine or loop (49, 54), a once-only block around
// the copy or the tile and 49, a pointer to mask 30, `mask = mask` 49. Tile's
// 0xfff from a function-scope `tileMask` makes the copy `tile = tileMask` that
// stops the inheritance, but without the REG_EQUAL note on `tile = 0xfff` that
// doubles tile's live length, tile outranks the row in global allocation and
// takes r7 (58, 68).
// Earlier rounds (2026-11-01 to 2026-11-08, 43 then 32): frame, loops, offset
// chain and the redraw block matched first; declaration order, signedness,
// operand order, compound assignments, the masks set before the load, cell
// reused for the word and permute runs (up to 10 minutes, 34 at best) did not
// move the cell section.
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
   * byte offset, offset += 2048), 167 of 148 instructions differ in tla-en.
   * tile is set to 0xfff and mask to 0x7ff before their ands; as
   * `tile = cell & 0xfff; mask = cell & 0x7ff;` (no asm), 35 of 148 differ. */
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
        u32 n;
        u32 column;
        u32 scaled;
        u32 mask;

        tile = 0xfff;
        mask = 0x7ff;
        /* FAKEMATCH(barrier): an empty asm on mask once it holds 0x7ff, which
         * gives mask's quantity the references that rank it above the
         * destination word in local allocation, so mask keeps r3, the word r2
         * and 0xfffff800 r1; as no asm, 49 of 148 instructions differ in
         * tla-en. */
        __asm__("" : "+r"(mask));
        tile = cell & tile;
        mask &= cell;
        mask |= *dst & 0xfffff800;
        *dst = mask;
        dst++;
        view = views;
        column = x & 15;
        i = 0;
        row = y & 15;
        n = row << 5;
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
