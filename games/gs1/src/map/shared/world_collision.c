#include "map.h"

extern struct MapState *Data_03001e70;

s32 CheckMapPositionCellOccupied(struct WorldPosition *position)
{
    s32 x;
    s32 y;
    s32 tile_x;
    s32 tile_y;
    struct MapState *work;
    u8 *cell;

    x = position->x / 65536;
    y = (position->y - *(s32 *)((u8 *)position + 4)) / 65536;
    work = Data_03001e70;
    if (work == NULL)
        return 0;
    cell = (u8 *)work->layers[2].cells;
    tile_x = x / 16;
    tile_y = y / 16;
    cell += (tile_x + tile_y * 128) * 4;
    return (cell[2] != 0xff) - 1;
}

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

s32 CheckWorldMapCollisionRange(s32 unused, struct WorldPosition *position)
{
    if ((u32)(GetWorldMapCollision(position)- 5) <= 7U) {
        return 0;
    }
    return -1;
}

extern u8 Data_0801353c[];

u8 GetWorldMapTerrainBehavior(struct WorldPosition *position, s32 *terrain_kind)
{
    s32 selector = GetWorldMapCollision(position);
    s32 x = position->x;
    s32 y;
    s32 flag = 0;
    u32 tile_x;
    u32 index;
    u32 *tile;

    if (x < 0)
        x += 0x1fffff;
    tile_x = (x >> 21) & 31;
    y = position->y;
    if (y < 0)
        y += 0x1fffff;

    index = tile_x + (((y >> 21) & 31) << 5);
    tile = (u32 *)0x02020000 + index;
    if (((u8 *)tile)[3] & 0x80)
        flag = 0x10;

    *terrain_kind = (*tile << 1) >> 25;
    if (*terrain_kind == 21)
        flag = 0x20;

    return Data_0801353c[flag + selector];
}
