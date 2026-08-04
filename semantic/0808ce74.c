#include "layout_guard.h"
#include "types.h"

struct SceneObject_0808ce74 {
    u8 unknown_00[6];
    u16 direction;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x0e];
    u8 terrain_id;
};

struct SceneRuntime_0808ce74 {
    u8 unknown_000[0x19e];
    s16 map_mode;
};

struct MapWork_0808ce74 {
    u8 unknown_000[0x130];
    u8 *tiles;
};

struct Position_0808ce74 {
    s32 x;
    s32 y;
    s32 z;
};

LAYOUT_OFFSET_GUARD(
    SceneObject0808ce74_Direction,
    struct SceneObject_0808ce74,
    direction,
    6);
LAYOUT_OFFSET_GUARD(
    SceneObject0808ce74_Position,
    struct SceneObject_0808ce74,
    x,
    8);
LAYOUT_OFFSET_GUARD(
    SceneObject0808ce74_TerrainId,
    struct SceneObject_0808ce74,
    terrain_id,
    0x22);
LAYOUT_OFFSET_GUARD(
    SceneRuntime0808ce74_MapMode,
    struct SceneRuntime_0808ce74,
    map_mode,
    0x19e);
LAYOUT_OFFSET_GUARD(
    MapWork0808ce74_Tiles,
    struct MapWork_0808ce74,
    tiles,
    0x130);
LAYOUT_SIZE_GUARD(
    Position0808ce74_Size,
    struct Position_0808ce74,
    12);

extern u32 Data_02000434;
extern u8 Data_02020000[];
extern struct SceneRuntime_0808ce74 *Data_03001ebc;
extern struct MapWork_0808ce74 *Data_03001e70;

struct SceneObject_0808ce74 *Func_0808ba1c(u32 object_id);
void Func_0800447c(
    s32 magnitude,
    s32 direction,
    struct Position_0808ce74 *position);
s32 Func_080091a8(s32 terrain_id, s32 x, s32 z);
void *Func_0808d48c(s32 descriptor_kind, s32 tile_kind);

/*
 * Return the special tile kind occupied by the selected scene object.
 *
 * The object's direction offsets the probe by one tile before map lookup.
 * Wrapped mode uses the fixed 32-by-32 map, while ordinary maps use their
 * runtime 128-wide tile table.  Tile kinds 0xf2 through 0xf7 additionally
 * require the terrain height to lie between the object's Y and four tiles
 * above it; other kinds require a matching class-3 map descriptor.
 */
u8 Func_0808ce74(void)
{
    struct SceneObject_0808ce74 *object;
    struct Position_0808ce74 position;
    u8 *tile;
    u8 tile_kind;
    s32 x;
    s32 z;

    object = Func_0808ba1c(Data_02000434);
    if (object == 0)
        return 0;

    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Func_0800447c(0x100000, object->direction, &position);

    if (Data_03001ebc->map_mode == 3) {
        x = position.x;
        if (x < 0)
            x += 0x1fffff;
        x = (x >> 21) & 31;

        z = position.z;
        if (z < 0)
            z += 0x1fffff;
        z = (z >> 21) & 31;

        tile = Data_02020000 + ((x + (z << 5)) * 4);
    } else {
        x = position.x;
        if (x < 0)
            x += 0x0fffff;
        x >>= 20;

        z = position.z;
        if (z < 0)
            z += 0x0fffff;
        z >>= 20;

        tile = Data_03001e70->tiles + ((x + (z << 7)) * 4);
    }

    tile_kind = tile[2];
    if ((u32)(tile_kind - 0xf2) <= 5) {
        s32 height = Func_080091a8(
            object->terrain_id,
            position.x,
            position.z);

        if (height >= object->y &&
            height <= object->y + 0x400000) {
            return tile_kind;
        }
    } else if (Func_0808d48c(3, tile_kind) != 0) {
        return tile_kind;
    }

    return 0;
}
