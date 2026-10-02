/* Ordinary map-attribute near miss: native/generated complete extent196 bytes.
 * B0 differs in49 bytes in full symbolic links for all six TLA editions;
 * both named pointer pools and the fffff000 mask are exact. The residual
 * is scheduling/register allocation, including shape-cursor increment order.
 * Both execute the same word cell accesses followed by shape-byte LDRB/STRB.
 * Width/height are s32; row/column are u16 with the native signed promotions.
 * Closed three ordinary forms: shared B0 preserves TBS132 all6; B1 emits
 * TLA196/46 but TBS132/4; B2 emits TLA196/155 but TBS144/126. Neither
 * improves the shared body. No fourth form or compiler device was added.
 * This is the frozen B0 body with only the approved physical shape-plane
 * name correction. Complete all-six linkage now closes the earlier missing
 * European physical bindings. The196-byte attribute owner remains raw;
 * only the separate72-byte wrapper is an exact COMMON adoption candidate.
 */
#include "EDITION.H"
#include "TYPES.H"

extern u8 gMapCellBuffer[];
extern u8 gMapShapeGrid[];

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
