#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "io_reg.h"

static void Map_DrawCellPanel(s32 x, s32 y, s32 width, s32 height, s32 plane,
                              s32 destX, s32 destY) {
  u32 *cell = (u32 *)Ram_MapCellBuffer + (y * 128 + x);
  s32 row;
  s32 col;
  s32 end;

  for (row = destY; row < destY + height; row++) {
    col = destX;
    if (col < destX + width)
      do {
        u16 tile = 0xfff & *cell++;
        u16 offset = tile * 8;
        s32 index = (plane * 16 + (row & 15)) * 32 + (col & 15);
        u32 *src;
        u32 *dst;

        dst = (u32 *)BG_SCREEN_BLOCK(5) + index;
        src = (u32 *)(Ram_MapBlocks + offset);
        *dst = *src;
        src = (u32 *)(Ram_MapBlocks + 4 + offset);
        dst = (u32 *)(BG_SCREEN_BLOCK(5) + 64) + index;
        *dst = *src;

        /* FAKEMATCH: forced temporary: end takes the bound where the loop tests
         * it, apart from the guard's destX + width, and the allocation follows
         * the bytes. Plain forms measured 2026-10-13 in both scenes that
         * include this file: ++col < destX + width differ 26 (the bound in r3
         * instead of r2 and the block addresses loaded in another order); col++
         * as its own expression differ 26; a for loop over col differ 26; end
         * set once before the guard and tested by both differ 54. */
      } while (end = destX + width, ++col < end);
    cell += 128 - width;
  }
}
