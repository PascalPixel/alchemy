// Draft, differ 105 of 87: the backward twin of Func_0802b590 (field/common/map/
// edit.c): copies a width by height block of map cells and their shape-grid
// bytes from (srcX, srcY) to (dstX, dstY), last row and column first so that a
// block overlapping its destination copies whole. Written in the forward
// twin's shape, with the column loop as a goto loop (133 with both for loops,
// where loop.c hoists the column start and the -0x10000 steps into a stack
// slot and r11). Left: the native's column loop is a real loop whose counter
// is strength-reduced (col << 16 stepped by 0xffff0000, read from the pool on
// every pass) while nothing is hoisted out of either loop.
#include "types.h"
#include "ram_buffer.h"

extern u8 gMapShapeGrid[];

void Func_0200217c(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                   s32 dstY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  s32 dst = (dstY << 7) + dstX;
  u8 *dstShapes = gMapShapeGrid + dst;
  s32 src = (srcY << 7) + srcX;
  u8 *srcShapes = gMapShapeGrid + src;
  u32 *dstCells = cells + dst;
  s16 row;
  s16 col;

  cells += src;
  for (row = height - 1; row >= 0; row--) {
    u32 *dstCell = dstCells + ((row << 7) + width) - 1;
    u32 *srcCell = cells + ((row << 7) + width) - 1;
    u8 *dstShape = dstShapes + ((row << 7) + width) - 1;
    u8 *srcShape = srcShapes + ((row << 7) + width) - 1;

    col = width - 1;
    if (col >= 0) {
    next:
      *dstCell-- = *srcCell--;
      *dstShape-- = *srcShape--;
      col--;
      if (col >= 0)
        goto next;
    }
  }
}
