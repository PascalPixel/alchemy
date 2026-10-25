// Draft, differ 133 of 87: the backward twin of Func_0802b590 (field/common/map/
// edit.c): copies a width by height block of map cells and their shape-grid
// bytes from (srcX, srcY) to (dstX, dstY), last row and column first so that a
// block overlapping its destination copies whole.
#include "types.h"
#include "ram_buffer.h"

extern u8 gMapShapeGrid[];

void Func_0200217c(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                   s32 dstY) {
  s32 dst = (dstY << 7) + dstX;
  s32 src = (srcY << 7) + srcX;
  u8 *dstShapes = gMapShapeGrid + dst;
  u8 *srcShapes = gMapShapeGrid + src;
  u32 *dstCells = (u32 *)Ram_MapCellBuffer + dst;
  u32 *srcCells = (u32 *)Ram_MapCellBuffer + src;
  s16 row;
  s16 col;

  for (row = height - 1; row >= 0; row--) {
    u32 *dstCell = dstCells + (row << 7) + width - 1;
    u32 *srcCell = srcCells + (row << 7) + width - 1;
    u8 *dstShape = dstShapes + (row << 7) + width - 1;
    u8 *srcShape = srcShapes + (row << 7) + width - 1;

    for (col = width - 1; col >= 0; col--) {
      *dstCell-- = *srcCell--;
      *dstShape-- = *srcShape--;
    }
  }
}
