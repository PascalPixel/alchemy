#include "edition.h"
#include "types.h"

extern u8 gMapCellBuffer[];
extern u8 gMapShapeGrid[];

#define MAP_CELLS ((u32 *)gMapCellBuffer)
#define MAP_CELL_TILE_MASK 0x00000fff
#define MAP_CELL_ATTRIBUTE_MASK 0xfffff000

void Map_CopyCellAttributeRect(
    s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y)
{
    u32 *dst = MAP_CELLS;
    u32 *src = dst;
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    u8 *dst_shape = gMapShapeGrid;
    u8 *src_shape = dst_shape;
#endif
    u16 row;
    u16 col;

    dst += (dst_y << 7) + dst_x;
    src += (src_y << 7) + src_x;
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    dst_shape += (dst_y << 7) + dst_x;
    src_shape += (src_y << 7) + src_x;
#endif

    for (row = 0; row < height; row++) {
        u32 *dst_cell = dst + (row << 7);
        u32 *src_cell = src + (row << 7);
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
        u8 *dst_shape_cell = dst_shape + (row << 7);
        u8 *src_shape_cell = src_shape + (row << 7);
#endif

        for (col = 0; col < width; col++) {
            *dst_cell = (*dst_cell & MAP_CELL_TILE_MASK) |
                (*src_cell++ & MAP_CELL_ATTRIBUTE_MASK);
            dst_cell++;
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
            *dst_shape_cell++ = *src_shape_cell++;
#endif
        }
    }
}
