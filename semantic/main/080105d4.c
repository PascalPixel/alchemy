#include "types.h"

struct RasterAnchor_080105d4 {
    s32 x;
    s32 y;
};

struct RasterObject_080105d4 {
    u8 pad00[4];
    s32 x;
    s32 y;
    u8 pad0c[0x24];
};

/*
 * Copy a tile-map rectangle to a second location and publish covered tiles on
 * the display layer assigned to each of the three active raster objects.
 */
void Func_080105d4(
    s32 source_x,
    s32 source_y,
    s32 width,
    s32 height,
    s32 destination_x,
    s32 destination_y)
{
    u32 *source = (u32 *)0x02010000 +
        source_y * 128 + source_x;
    u32 *destination = (u32 *)0x02010000 +
        destination_y * 128 + destination_x;
    struct RasterObject_080105d4 *object =
        (struct RasterObject_080105d4 *)
        (*(u8 **)0x03001E70 + 0x104);
    struct RasterAnchor_080105d4 anchors[3];
    s32 row;

    for (row = 0; row < 3; row++, object++) {
        anchors[row].x = object->x >> 20;
        anchors[row].y = object->y >> 20;
    }

    for (row = 0; row < height; row++) {
        s32 y = destination_y + row;
        s32 column;

        for (column = 0; column < width; column++) {
            s32 x = destination_x + column;
            u32 source_entry = *source++;
            u32 tile = source_entry & 0xFFF;
            u32 screen_offset =
                (((y & 15) << 5) + (x & 15)) * sizeof(u32);
            s32 layer;

            *destination++ = source_entry;

            for (layer = 0; layer < 3; layer++) {
                if (x >= anchors[layer].x &&
                    x < anchors[layer].x + 16 &&
                    y >= anchors[layer].y &&
                    y < anchors[layer].y + 12) {
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
