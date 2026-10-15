#include "edition.h"
#include "types.h"

extern u8 gMapCellBuffer[];
extern u8 gMapShapeGrid[];

#define MAP_CELLS ((u32 *)gMapCellBuffer)
#define MAP_CELL_TILE_MASK 0x00000fff
#define MAP_CELL_ATTRIBUTE_MASK 0xfffff000

void Map_CopyCellAttributeRect(
    s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX, s32 dstY)
{
    u32 *dst = MAP_CELLS;
    u32 *src = dst;
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    u8 *dstShape = gMapShapeGrid;
    u8 *srcShape = dstShape;
#endif
    u16 row;
    u16 col;

    dst += (dstY << 7) + dstX;
    src += (srcY << 7) + srcX;
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    dstShape += (dstY << 7) + dstX;
    srcShape += (srcY << 7) + srcX;
#endif

    for (row = 0; row < height; row++) {
        u32 *dstCell = dst + (row << 7);
        u32 *srcCell = src + (row << 7);
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
        u8 *dstShapeCell = dstShape + (row << 7);
        u8 *srcShapeCell = srcShape + (row << 7);
#endif

        for (col = 0; col < width; col++) {
            *dstCell = (*dstCell & MAP_CELL_TILE_MASK) |
                (*srcCell++ & MAP_CELL_ATTRIBUTE_MASK);
            dstCell++;
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
            *dstShapeCell++ = *srcShapeCell++;
#endif
        }
    }
}
