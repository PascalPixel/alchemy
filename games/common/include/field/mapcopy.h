#ifndef ALCHEMY_MAPCOPY_H
#define ALCHEMY_MAPCOPY_H

#include "edition.h"
#include "types.h"

#if GAME_TLA
void Map_CopyMetatileIndicesRect(s32 srcX, s32 srcY, s32 dst_x, s32 dst_y,
                                 s32 width, s32 height);
void Map_CopyCellsRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 dst_x,
                       s32 dst_y);
void Map_CopyCellAttributeRect(s32 srcX, s32 srcY, s32 width, s32 height,
                               s32 dst_x, s32 dst_y);
#endif

#endif
