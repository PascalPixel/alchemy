#include "types.h"

struct RasterAnchor_08010424 {
    s32 x;
    s32 y;
};

struct RasterObject_08010424 {
    u8 pad00[4];
    s32 x;
    s32 y;
    u8 pad0c[0x24];
};

/*
 * Copy a rectangular tile-map region and expose tiles covered by one of the
 * three active raster objects on its corresponding display layer.
 */
void Func_08010424(
    u16 source_x,
    u16 source_y,
    u32 destination_x,
    u32 destination_y,
    u32 width,
    u32 height)
{
    u32 *source = (u32 *)0x02010000 +
        source_y * 128 + source_x;
    u32 *destination = (u32 *)0x02010000 +
        destination_y * 128 + destination_x;
    struct RasterObject_08010424 *object =
        (struct RasterObject_08010424 *)
        (*(u8 **)0x03001E70 + 0x104);
    struct RasterAnchor_08010424 anchors[3];
    u32 row;

    for (row = 0; row < 3; row++, object++) {
        anchors[row].x = object->x >> 20;
        anchors[row].y = object->y >> 20;
    }

    for (row = 0; row < height; row++) {
        u32 y = destination_y + row;
        u32 column;

        for (column = 0; column < width; column++) {
            u32 x = destination_x + column;
            s32 signed_x = (s32)x;
            s32 signed_y = (s32)y;
            u32 source_entry = *source++;
            u32 tile = source_entry & 0xFFF;
            u32 screen_offset =
                (((y & 15) << 5) + (x & 15)) * sizeof(u32);
            u32 layer;

            *destination = (*destination & 0xFFFFF000) | tile;
            destination++;

            for (layer = 0; layer < 3; layer++) {
                if (signed_x >= anchors[layer].x &&
                    signed_x < anchors[layer].x + 16 &&
                    signed_y >= anchors[layer].y &&
                    signed_y < anchors[layer].y + 12) {
                    *(u32 *)(0x06002800 + screen_offset) =
                        *(u32 *)(0x02020000 + tile * 8);
                    *(u32 *)(0x06002840 + screen_offset) =
                        *(u32 *)(0x02020004 + tile * 8);
                    break;
                }
                screen_offset += 0x800;
            }
        }

        source += 128 - width;
        destination += 128 - width;
    }
}
