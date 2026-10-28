/// @file
/// Map: copy a cell rectangle.

#include "edition.h"
#include "mapcopy.h"


/// Copies a rectangle's cells, including block indices and attributes.
/// srcX, srcY: source top left, in cells.
/// width, height: size in cells.
/// dstX, dstY: destination top left, in cells.
void Map_CopyCellsRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                       s32 dstY) {
  Map_CopyCellPictureIndicesRect(srcX, srcY, dstX, dstY, width, height);
  Map_CopyCellAttributeRect(srcX, srcY, width, height, dstX, dstY);
}
