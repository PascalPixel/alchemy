#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "RAM_BUFFER.H"
#include "MAP.H"
#include "OBJECT_RUNTIME.H"

extern u8 gMapCellBuffer[];

/* A metatile's attributes are four bytes: its shape in the low nibble of
 * the first, then the height bytes its shape's function reads. */
typedef s32 (*TerrainHeightFn)(u8 *heights, s32 x, s32 y);

/* The height function of each of the sixteen shapes. */
extern TerrainHeightFn Map_TerrainHeightFunctions[16];

/* Height of the terrain at a 16.16 map position on one of the layers: the
 * metatile's shape selects the height function, which gets the metatile's
 * height bytes and the position inside the metatile. */
s32 Func_08011f54(s32 layer, s32 x, s32 y)
{
    struct MapState *work = gMapWork[0];
    u8 *cells;
    u8 *attributes;

    x >>= 16;
    y >>= 16;
    cells = Ram_MapCellBuffer;
    if (work != NULL) {
        cells = (u8 *)work->layers[layer & 3].cells;
    }
    cells += (x / 16 + (y / 16 << 7)) * 4;
    attributes = Ram_MapCollision + cells[3] * 4;
    return Map_TerrainHeightFunctions[*attributes++ & 15](attributes, x & 15, y & 15);
}

s32 Map_GetCellAttributeLowNibble(s32 index, s32 x, s32 y)
{
    struct MapState *state = gMapWork[0];
    u8 *map;
    s32 col;
    s32 row;
    u32 attr;

    x >>= 16;
    y >>= 16;
    map = (u8 *)gMapCellBuffer;
    if (state != 0) {
        map = (u8 *)state->layers[index & 3].cells;
    }
    col = x / 16;
    row = y / 16;
    map += (col + (row << 7)) * 4;
    attr = map[3];
    return *(Ram_MapCollision + attr * 4) & 15;
}

u8 GetMapCellCollision(s32 layer, s32 x, s32 y)
{
    struct MapState *state;
    s32 cell_address;

    state = gMapWork[0];
    x >>= 20;
    y >>= 20;
    cell_address = (u32)gMapCellBuffer;
    if (state != NULL) {
        cell_address = (s32)state->layers[layer & 3].cells;
    }
    cell_address += (x + (y << 7)) * sizeof(struct MapCell);
    return ((struct MapCell *)cell_address)->collision_code;
}

void SetMapCellCollision(u32 layer, s32 x, s32 y, u32 collision_code)
{
    /* FAKEMATCH: retain the existing cell-byte write. The typed element
       store in aadcc37dd reversed the add operands and selected r3, not r2. */
    struct MapState *state = gMapWork[0];

    x >>= 20;
    y >>= 20;
    if (state != NULL) {
        struct MapCell *cells = state->layers[layer & 3].cells;
        u32 offset = (x + (y << 7)) * sizeof(struct MapCell);
        u8 *cell = (u8 *)cells;

        cell += offset;
        cell[(u32)&((struct MapCell *)0)->collision_code] = collision_code;
    }
}

s32 Map_GetCellHighFlags(s32 x, s32 y)
{
    s32 tile_x = x / 16;
    s32 tile_y = y / 16;
    u8 *cell = (u8 *)gMapCellBuffer + (tile_x + tile_y * 128) * 4;

    return cell[1] >> 6;
}

/* The fields of a field object the terrain test reads. */

/* A 16.16 position: the terrain test reads only its whole x and z. */
struct TerrainPosition {
    u8 unknown_00[2];
    s16 x;
    u8 unknown_04[6];
    s16 z;
};

/*
 * Compare the terrain at a position with an object's height: 2 when the
 * cell is closed to walking, 1 when the terrain is more than half a tile
 * above the object, -1 when it is more than three quarters of a tile
 * below, 0 when the object can step there or no map is loaded.
 */
s32 Func_080120dc(struct ObjectRuntime *object, struct TerrainPosition *position)
{
    struct MapState *state;
    s32 x;
    s32 z;
    u8 *cell;
    u8 *attributes;
    s32 height;
    s32 delta;

    x = position->x;
    z = position->z;
    state = gMapWork[0];
    if (state == NULL)
        return 0;
    if (object->terrain_id <= 2)
        cell = (u8 *)state->layers[object->terrain_id].cells;
    else
        cell = Ram_MapCellBuffer;
    cell += (x / 16 + (z / 16 << 7)) * 4;
    if (cell[2] == 0xff)
        return 2;
    attributes = Ram_MapCollision + cell[3] * 4;
    height = Map_TerrainHeightFunctions[*attributes++ & 15](attributes, x & 15, z & 15);
    delta = height - object->terrain_height;
    if (delta > 0x80000)
        return 1;
    if (delta < (s32)0xfff40000)
        return -1;
    return 0;
}
