#ifndef ALCHEMY_MAPCOPY_H
#define ALCHEMY_MAPCOPY_H

#include "edition.h"
#include "types.h"

void MapGrid_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, u32 dstX, u32 dstY,
                                        u32 width, u32 height);
void Map_CopyCellsRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 dst_x,
                       s32 dst_y);
void Map_CopyCellAttributeRect(s32 srcX, s32 srcY, s32 width, s32 height,
                               s32 dst_x, s32 dst_y);

#endif
