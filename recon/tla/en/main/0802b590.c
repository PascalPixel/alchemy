#include "types.h"
#include "ram_buffer.h"

extern u8 gMapShapeGrid[];

void Map_CopyCellShapeRect(
    s32 srcX, s32 srcY, s32 width, s32 height, s32 dst_x, s32 dst_y)
{
    u32 *cells = (u32 *)Ram_MapCellBuffer;
    u8 *shapes = gMapShapeGrid;
    s32 dst = (dst_y << 7) + dst_x;
    s32 src = (srcY << 7) + srcX;
    u8 *dst_shapes = shapes + dst;
    u32 *dst_cells = cells + dst;
    u8 *src_shapes = shapes + src;
    u32 *src_cells = cells + src;
    u16 row;
    u16 col;

    for (row = 0; row < height; row++) {
        u32 *dst_cell = dst_cells + (row << 7);
        u32 *src_cell = src_cells + (row << 7);
        u8 *dst_shape = dst_shapes + (row << 7);
        u8 *src_shape = src_shapes + (row << 7);

        for (col = 0; col < width; col++) {
            *dst_cell++ = *src_cell++;
            *dst_shape++ = *src_shape++;
        }
    }
}
