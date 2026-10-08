#include "ram_buffer.h"
#include "types.h"
#include "map.h"
#include "io_reg.h"

s32 GetWorldMapCollision(struct WorldPosition *position)
{
    s32 x_step = position->x >> 17;
    s32 y = position->y;
    s32 y_step = y >> 17;
    u32 cell;
    u32 pixel_offset;
    u8 tile;
    u8 packed_pixels;
    u32 result;

    cell = (((y_step / 8) & 63) << 6) + ((x_step / 8) & 63);
    pixel_offset = (((y_step / 2) & 3) << 1) + ((x_step / 4) & 1);

    tile = BG_SCREEN_BLOCK(10)[cell];
    packed_pixels = *(Ram_MapCollision + 0x800 + (tile << 3) + pixel_offset);
    if (packed_pixels != 0) {
        if (x_step & 2)
            result = packed_pixels >> 4;
        else
            result = packed_pixels & 15;
        if (result != 0)
            return result;
    }

    tile = BG_SCREEN_BLOCK(8)[cell];
    packed_pixels = *(Ram_MapCollision + (tile << 3) + pixel_offset);
    if (packed_pixels != 0) {
        if (x_step & 2)
            result = packed_pixels >> 4;
        else
            result = packed_pixels & 15;
        if (result != 0)
            return result;
    }

    return 7;
}
