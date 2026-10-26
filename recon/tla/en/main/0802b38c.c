// Draft, differ 2 of 92 in tla-en under -mthumb-slow-loads (2026-10-26). No
// single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer, nor any pair of empty asm statements or
// one-pass blocks (device search).
// Near miss, differ 4 of 92 (2026-10-18): copies the attributes of a
// rectangle of map cells (keeping each destination's block number) and their
// shape grid bytes, written as its matched twin Func_0802b590 (edit.c):
// the destination's shape pointer before the source index, the shape grid
// addressed afresh for each, the cell base advanced to the source in place.
// Left: the native loads the attribute mask 0xfffff000 one step earlier, and
// in the loop builds the column step 0x10000 before reading the shape byte;
// here the read comes first. The step is the loop's col << 16 counter, so
// moving col++ ahead of the shape copy, with or without an empty asm before
// the read, leaves 4; eight minutes of permute: no better (2026-10-19).
#include "types.h"
#include "ram_buffer.h"

extern u8 gMapShapeGrid[];

#define MAP_CELL_TILE_MASK 0x00000fff
#define MAP_CELL_ATTRIBUTE_MASK 0xfffff000

void Map_CopyCellAttributeRect(s32 srcX, s32 srcY, s32 width, s32 height,
                               s32 dstX, s32 dstY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  s32 dst = (dstY << 7) + dstX;
  u8 *dstShapes = gMapShapeGrid + dst;
  s32 src = (srcY << 7) + srcX;
  u8 *srcShapes = gMapShapeGrid + src;
  u32 *dstCells = cells + dst;
  u16 row;
  u16 col;

  cells += src;
  for (row = 0; row < height; row++) {
    u32 *dstCell = dstCells + (row << 7);
    u32 *srcCell = cells + (row << 7);
    u8 *dstShape = dstShapes + (row << 7);
    u8 *srcShape = srcShapes + (row << 7);

    for (col = 0; col < width; col++) {
      *dstCell = (*dstCell & MAP_CELL_TILE_MASK) |
                 (*srcCell++ & MAP_CELL_ATTRIBUTE_MASK);
      dstCell++;
      *dstShape = *srcShape;
      dstShape++;
      srcShape++;
    }
  }
}
