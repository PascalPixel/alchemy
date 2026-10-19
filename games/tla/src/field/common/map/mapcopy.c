//--------------------------------------------------------------------------
// mapcopy.c
// Map: copy a cell rectangle.
//--------------------------------------------------------------------------

#include "edition.h"
#include "mapcopy.h"


//--------------------------------------------------------------------------
// Copy a rectangle's cells, including block indices and attributes.
// srcX, srcY: source top left, in cells.
// width, height: size in cells.
// dst_x, dst_y: destination top left, in cells.
//--------------------------------------------------------------------------
void Map_CopyCellsRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                       s32 dstY) {
  Map_CopyMetatileIndicesRect(srcX, srcY, dstX, dstY, width, height);
  Map_CopyCellAttributeRect(srcX, srcY, width, height, dstX, dstY);
}
