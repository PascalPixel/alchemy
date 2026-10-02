#ifndef ALCHEMY_MAPCOPY_H
#define ALCHEMY_MAPCOPY_H

#include "EDITION.H"
#include "TYPES.H"

#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
void Map_CopyMetatileIndicesRect(
    s32 src_x, s32 src_y, s32 dst_x, s32 dst_y, s32 width, s32 height);
void Map_CopyCellsRect(
    s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y);
#endif
void Map_CopyCellAttributeRect(
    s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y);

#endif
