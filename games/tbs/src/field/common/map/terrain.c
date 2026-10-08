#include "types.h"
#include "global_cells.h"
#include "ram_buffer.h"
#include "map.h"
#include "object_runtime.h"
#include "fieldrun.h"
#include "fieldobj.h"

extern u8 gMapCellBuffer[];

typedef s32 (*TerrainHeightFn)(u8 *heights, s32 x, s32 y);

extern TerrainHeightFn Map_TerrainHeightFunctions[16];

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 y)
{
    struct MapState *work = gMapWork[0];
    struct MapCell *cells;
    u8 *attributes;

    x >>= 16;
    y >>= 16;
    cells = (struct MapCell *)Ram_MapCellBuffer;
    if (work != NULL) {
        cells = work->layers[layer & 3].cells;
    }
    cells += x / 16 + (y / 16 << 7);
    attributes = Ram_MapCollision + cells->attribute_b * 4;
    return Map_TerrainHeightFunctions[*attributes++ & 15](attributes, x & 15, y & 15);
}

s32 Map_GetCellAttributeLowNibble(s32 index, s32 x, s32 y)
{
    struct MapState *state = gMapWork[0];
    struct MapCell *map;
    s32 col;
    s32 row;
    u32 attr;

    x >>= 16;
    y >>= 16;
    map = (struct MapCell *)gMapCellBuffer;
    if (state != 0) {
        map = state->layers[index & 3].cells;
    }
    col = x / 16;
    row = y / 16;
    map += col + (row << 7);
    attr = map->attribute_b;
    return *(Ram_MapCollision + attr * 4) & 15;
}

u8 GetMapCellCollision(s32 layer, s32 x, s32 y)
{
    struct MapState *state;
    struct MapCell *cells;

    state = gMapWork[0];
    x >>= 20;
    y >>= 20;
    cells = (struct MapCell *)gMapCellBuffer;
    if (state != NULL) {
        cells = state->layers[layer & 3].cells;
    }
    cells += x + (y << 7);
    return cells->collision_code;
}

void SetMapCellCollision(u32 layer, s32 x, s32 y, u32 collision_code)
{
    struct MapState *state = gMapWork[0];

    x >>= 20;
    y >>= 20;
    if (state != NULL) {
        struct MapCell *cells = state->layers[layer & 3].cells;

        cells += x + (y << 7);
        cells->collision_code = collision_code;
    }
}

s32 Map_GetCellHighFlags(s32 x, s32 y)
{
    s32 tile_x = x / 16;
    s32 tile_y = y / 16;
    struct MapCell *cell = (struct MapCell *)gMapCellBuffer + (tile_x + tile_y * 128);

    return ((u8 *)&cell->metatile_and_flags)[1] >> 6;
}

s32 Map_CheckObjectStep(struct ObjectRuntime *object, struct FieldPosition *position)
{
    struct MapState *state;
    s32 x;
    s32 z;
    struct MapCell *cell;
    u8 *attributes;
    s32 height;
    s32 delta;

    x = ((union FieldCoordinate *)&position->x)->part.pixel;
    z = ((union FieldCoordinate *)&position->z)->part.pixel;
    state = gMapWork[0];
    if (state == NULL)
        return 0;
    if (object->terrain_id <= 2)
        cell = state->layers[object->terrain_id].cells;
    else
        cell = (struct MapCell *)Ram_MapCellBuffer;
    cell += x / 16 + (z / 16 << 7);
    if (cell->collision_code == 0xff)
        return 2;
    attributes = Ram_MapCollision + cell->attribute_b * 4;
    height = Map_TerrainHeightFunctions[*attributes++ & 15](attributes, x & 15, z & 15);
    delta = height - object->terrain_height;
    if (delta > 0x80000)
        return 1;
    if (delta < -0xc0000)
        return -1;
    return 0;
}
