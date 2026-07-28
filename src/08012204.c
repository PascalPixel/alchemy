#include "types.h"
#include "map.h"

s32 Func_08012204(struct WorldPosition *position)
{
    s32 x_step = position->x >> 17;
    s32 y = position->y;
    s32 y_step = y >> 17;
    u32 cell;
    u32 pixel_offset;
    u8 tile;
    u8 packed_pixels;
    u32 result;

    /* 1行64バイトで衝突判定タイル64個を表す。 */
    cell = (((u32)(y_step / 8) & 63) << 6) +
        ((u32)(x_step / 8) & 63);
    /* 4×4ドット・4bppの衝突判定タイルは8バイト。 */
    pixel_offset = (((u32)(y_step / 2) & 3) << 1) +
        ((u32)(x_step / 4) & 1);

    tile = *(u8 *)(0x06005000 + cell);
    packed_pixels = *(u8 *)(0x0202c800 + ((u32)tile << 3) + pixel_offset);
    if (packed_pixels != 0) {
        if ((u32)x_step & 2)
            result = packed_pixels >> 4;
        else
            result = packed_pixels & 15;
        if (result != 0)
            return result;
    }

    /* 前面が空なら背面マップを調べる。 */
    tile = *(u8 *)(0x06004000 + cell);
    packed_pixels = *(u8 *)(0x0202c000 + ((u32)tile << 3) + pixel_offset);
    if (packed_pixels != 0) {
        if ((u32)x_step & 2)
            result = packed_pixels >> 4;
        else
            result = packed_pixels & 15;
        if (result != 0)
            return result;
    }

    return 7;
}
