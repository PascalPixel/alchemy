#include "EDITION.H"
#include "MAPCOPY.H"

#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
/* Only this complete wrapper is source-owned in TLA; its index and
   attribute callees remain raw at their existing neighboring boundaries. */
void Map_CopyCellsRect(
    s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y)
{
    Map_CopyMetatileIndicesRect(src_x, src_y, dst_x, dst_y, width, height);
    Map_CopyCellAttributeRect(src_x, src_y, width, height, dst_x, dst_y);
}
#else
extern u8 gMapCellBuffer[];

#define MAP_CELLS ((u32 *)gMapCellBuffer)
#define MAP_CELL_TILE_MASK 0x00000fff
#define MAP_CELL_ATTRIBUTE_MASK 0xfffff000


/* Copies a width by height block of map cells from (src_x, src_y) to
 * (dst_x, dst_y) in the 128-cell-wide map grid, replacing everything above
 * each destination cell's low 12 bits and keeping those. */
void Map_CopyCellAttributeRect(
    s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y)
{
    u32 *dst = MAP_CELLS;
    u32 *src = dst;
    u16 row;
    u16 col;

    dst += (dst_y << 7) + dst_x;
    src += (src_y << 7) + src_x;

    for (row = 0; row < height; row++) {
        u32 *dst_cell = dst + (row << 7);
        u32 *src_cell = src + (row << 7);

        for (col = 0; col < width; col++) {
            *dst_cell = (*dst_cell & MAP_CELL_TILE_MASK) |
                (*src_cell++ & MAP_CELL_ATTRIBUTE_MASK);
            dst_cell++;
        }
    }
}
#endif
