#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "map.h"

/* map/shared/get_cell_attribute_low_nibble.c */
s32 Map_GetCellAttributeLowNibble(s32 index, s32 x, s32 y)
{
    u8 *state = *(u8 **)ADDR_03001E70;
    u8 *map;
    s32 off;
    s32 col;
    s32 row;
    u32 attr;

    x >>= 16;
    y >>= 16;
    map = (u8 *)0x02010000;
    if (state != 0) {
        off = (index & 3) * 48 + 304;
        map = *(u8 **)(state + off);
    }
    col = x / 16;
    row = y / 16;
    map += (col + (row << 7)) * 4;
    attr = map[3];
    return *(u8 *)(0x0202c000 + attr * 4) & 15;
}

/* map/shared/get_cell_collision.c */
u8 GetMapCellCollision(s32 layer, s32 x, s32 y)
{
    struct MapState *state;
    s32 cell_address;
    s32 layer_offset;

    state = *(struct MapState **)ADDR_03001E70;
    x >>= 20;
    y >>= 20;
    cell_address = 0x02010000;
    if (state != 0) {
        layer_offset = ((layer & 3) * sizeof(struct MapLayer)) + 0x130;
        cell_address = *(s32 *)((u8 *)state + layer_offset);
    }
    cell_address += (x + (y << 7)) * sizeof(struct MapCell);
    return ((struct MapCell *)cell_address)->collision_code;
}

/* map/shared/set_cell_collision.c */
extern struct MapState *gCam;

void SetMapCellCollision(u32 layer, s32 x, s32 y, u32 collision_code)
{
    struct MapState *state = gCam;

    x >>= 20;
    y >>= 20;
    if (state != 0) {
        struct MapCell *cells = state->layers[layer & 3].cells;
        u32 offset = (x + (y << 7)) * sizeof(struct MapCell);
        u8 *cell = (u8 *)cells;

        cell += offset;
        cell[2] = collision_code;
    }
}

/* map/shared/get_cell_high_flags.c */
s32 Map_GetCellHighFlags(s32 x, s32 y)
{
    s32 tile_x = x / 16;
    s32 tile_y = y / 16;
    u8 *cell = (u8 *)0x02010000 + (tile_x + tile_y * 128) * 4;

    return cell[1] >> 6;
}

/* map/shared/check_position_cell_occupied.c */
struct MapCollisionWork {
    u8 unknown_000[0x190];
    u8 *cells;
};

s32 CheckMapPositionCellOccupied(struct WorldPosition *position)
{
    s32 x;
    s32 y;
    s32 tile_x;
    s32 tile_y;
    struct MapCollisionWork *work;
    u8 *cell;

    x = position->x / 65536;
    y = (position->y - *(s32 *)((u8 *)position + 4)) / 65536;
    work = *(struct MapCollisionWork **)ADDR_03001E70;
    if (work == NULL)
        return 0;
    cell = work->cells;
    tile_x = x / 16;
    tile_y = y / 16;
    cell += (tile_x + tile_y * 128) * 4;
    return (cell[2] != 0xFF) - 1;
}

/* map/shared/get_world_map_collision.c */
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

/* map/shared/check_world_map_collision_range.c */
struct WorldPosition;

s32 CheckWorldMapCollisionRange(s32 unused, struct WorldPosition *position)
{
    if ((u32)(GetWorldMapCollision(position)- 5) <= 7U) {
        return 0;
    }
    return -1;
}

/* map/shared/get_world_map_terrain_behavior.c */
extern u8 gRom[];

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

    return gRom[flag + selector];
}
