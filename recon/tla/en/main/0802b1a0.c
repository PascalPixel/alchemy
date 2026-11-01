// Draft, differ 2 of 142, skeleton gap 1 (tla-en, 2026-11-01)
// Twin of The Broken Seal's MapGrid_CopyCellPictureIndicesRect: same layer
// scroll read, same 16x12 window test on three layers, same 2,048-byte BG map
// step from screen block 5, the same two word copies from the cell pictures.
// Left: one scheduling pair. The native copies x for the mask straight after
// the stmia (adds r2, r1 first, then the destination spill, mov r3, sl and
// mov r4, ip); here that copy comes last of the four. Allocation, frame and
// everything else agree. In the sched2 dump all three copies and mov r4, ip
// have priority 4; the destination copy is taken first, the mask copy last.
// The gain from differ 150 came from using ram_buffer.h's Ram_MapBlocks, the
// fixed integer address the twin uses: as an extern symbol CSE derived +4 from
// a hoisted base, which took a register and pushed dstX onto the stack. The
// volatile casts on the two word copies are gone; they changed nothing.
// Tried, all still differ 2 or worse: the mask before, after or split around
// the store, 15 & x, swapped dst++ and tmp6, *dst++, the ior swapped, i = 0
// and pos = tile moved, tmp12, tmp6 and x/y declaration order, y % 16.
// Worse: x % 16 (8), tmp6 = x & 15 directly (8), no s32 casts (10).
#include "types.h"
#include "ram_buffer.h"
#include "map.h"

struct TilePos {
  s32 x, y;
};

void Map_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, u32 dstX, u32 dstY,
                                 u32 width, u32 height) {
  /* FAKEMATCH(temporary): the twin's chains of temporaries set the registers
   * and the schedule of the cell loop; as a plain body, 96 of 141 instructions
   * differ in tbs-en (the twin's measure, on its own function). */
  u32 *src = (u32 *)Ram_MapCellBuffer + (srcY * 128 + srcX);
  u32 *dst = (u32 *)Ram_MapCellBuffer + (dstY * 128 + dstX);
  struct MapLayer *layer = Ram_HeapSlots->mapWork->layers;
  struct TilePos tile[3];
  s32 i;
  struct TilePos *pos = tile;
  u32 y;
  u32 x;

  for (i = 2; i >= 0; i--) {
    pos->x = layer->x >> 20;
    pos->y = layer->y >> 20;
    pos++;
    layer++;
  }
  for (y = dstY; y < dstY + height; y++) {
    for (x = dstX; x < dstX + width; x++) {
      u32 val = *src++;
      u32 cell = val & 0xfff;
      u32 offset;
      u32 tmp3;
      u32 n;
      u32 tmp6;
      u32 tmp9;
      u32 tmp12;
      u32 tmp11;
      u32 tmp13;
      u32 tmp15;
      tmp9 = *dst;
      tmp3 = tmp9 & -0x1000;
      tmp12 = x & 15;
      dst[0] = cell | tmp3;
      tmp6 = tmp12;
      dst++;
      i = 0;
      tmp15 = y & 15;
      tmp3 = tmp15;
      pos = tile;
      tmp13 = tmp3 << 5;
      tmp12 = tmp13;
      n = tmp12;
      n += tmp6;
      tmp13 = 4 * n;
      tmp11 = tmp13;
      tmp6 = tmp11;
      offset = tmp6;
      for (; i < 3; i++) {
        if (pos->x <= (s32)x && 16 + pos->x > (s32)x && pos->y <= (s32)y &&
            pos->y + 12 > (s32)y) {
          u32 scr, blk;
          u8 *dest, *tiles;

          scr = 0x06002800;
          dest = (u8 *)scr;
          dest += offset;
          tmp11 = cell * 8;
          blk = (u32)Ram_MapBlocks;
          tiles = (u8 *)blk;
          tiles += tmp11;
          *(u32 *)dest = *(u32 *)tiles;

          blk = (u32)Ram_MapBlocks + 4;
          tiles = (u8 *)blk;
          tiles += tmp11;
          scr = 0x06002840;
          dest = (u8 *)scr;
          dest += offset;
          *(u32 *)dest = *(u32 *)tiles;
          break;
        }
        offset += 0x800;
        pos++;
      }
    }
    src += 128 - width;
    dst += 128 - width;
  }
}
