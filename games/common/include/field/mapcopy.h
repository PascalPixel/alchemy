#ifndef ALCHEMY_MAPCOPY_H
#define ALCHEMY_MAPCOPY_H

#include "edition.h"
#include "types.h"

#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) ||                      \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) ||                      \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#if GAME_TLA
void Map_CopyMetatileIndicesRect(s32 srcX, s32 srcY, s32 dst_x, s32 dst_y,
                                 s32 width, s32 height);
#endif
void Map_CopyCellsRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 dst_x,
                       s32 dst_y);
#endif
#if GAME_TLA
void Map_CopyCellAttributeRect(s32 srcX, s32 srcY, s32 width, s32 height,
                               s32 dst_x, s32 dst_y);
#endif

#endif
