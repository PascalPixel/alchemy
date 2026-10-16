#include "types.h"
#include "ram_buffer.h"

extern u8 gMapShapeGrid[];

void Map_CopyCellShapeRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                           s32 dstY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  u8 *shapes = gMapShapeGrid;
  s32 dst = (dstY << 7) + dstX;
  s32 src = (srcY << 7) + srcX;
  u8 *dstShapes = shapes + dst;
  u32 *dstCells = cells + dst;
  u8 *srcShapes = shapes + src;
  u32 *srcCells = cells + src;
  u16 row;
  u16 col;

  for (row = 0; row < height; row++) {
    u32 *dstCell = dstCells + (row << 7);
    u32 *srcCell = srcCells + (row << 7);
    u8 *dstShape = dstShapes + (row << 7);
    u8 *srcShape = srcShapes + (row << 7);

    for (col = 0; col < width; col++) {
      *dstCell++ = *srcCell++;
      *dstShape++ = *srcShape++;
    }
  }
}
