#include "types.h"

struct RasterAnchor_08010424 {
    s32 x;
    s32 y;
};

struct RasterObject_08010424 {
    s32 x;
    s32 y;
    u8 pad08[0x28];
};

/*
 * Copy a rectangular tile-map region and expose tiles covered by one of the
 * three active raster objects on its corresponding display layer.
 */
void Func_08010424(
    u32 source_x,
    u32 source_y,
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
    struct RasterAnchor_08010424 *anchor;
    u32 layer;
    u32 x;
    u32 y;

    for (layer = 0, anchor = anchors; layer < 3;
         layer++, object++, anchor++) {
        anchor->x = object->x >> 20;
        anchor->y = object->y >> 20;
    }

    for (y = destination_y; y < destination_y + height; y++) {
        for (x = destination_x; x < destination_x + width; x++) {
            u32 source_entry = *source++;
            u32 tile = source_entry & 0xFFF;
            u32 screen_offset =
                (((y & 15) << 5) + (x & 15)) * sizeof(u32);

            *destination = (*destination & 0xFFFFF000) | tile;
            destination++;

            for (layer = 0, anchor = anchors; layer < 3;
                 layer++, anchor++) {
                if ((s32)x >= anchor->x &&
                    (s32)x < anchor->x + 16 &&
                    (s32)y >= anchor->y &&
                    (s32)y < anchor->y + 12) {
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
