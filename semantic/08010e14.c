#include "types.h"

struct SceneAssets_08010e14 {
    s32 palette;
    s32 tiles_0;
    s32 tiles_1;
    s32 tiles_2;
    s32 tiles_3;
    s32 map;
};

struct SceneState_08010e14 {
    u8 unknown_000[0x11C];
    const struct SceneAssets_08010e14 *assets;
    u8 unknown_120[0x2C];
    u16 first_special_tiles[3];
    u8 unknown_152[0x1A];
    u16 second_special_tiles[3];
};

struct Dma3_08010e14 {
    const void *source;
    void *destination;
    u32 control;
};

void *Func_08004938(u32);
void Func_08002df0(void *);
const void *Func_08002f40(s32);
void Func_08005340(const void *, void *);
void Func_080053e8(const void *, void *);
void Func_080113e4(void);

/*
 * Load the scene graphics selected by the world-map cell at (world_x,
 * world_y).  A cell property chooses one of two six-resource bundles.
 */
void Func_08010e14(s32 world_x, s32 world_y) {
    u16 *palette_buffer = Func_08004938(0x200);
    struct SceneState_08010e14 *state =
        *(struct SceneState_08010e14 **)0x03001E70;
    volatile struct Dma3_08010e14 *dma =
        (volatile struct Dma3_08010e14 *)0x040000D4;
    const struct SceneAssets_08010e14 *asset_sets =
        (const struct SceneAssets_08010e14 *)0x080132CC;
    const struct SceneAssets_08010e14 *assets;
    s32 cell_x = world_x;
    s32 cell_y = world_y;
    u32 map_entry;
    s32 special;
    s16 saved_backdrop;
    u32 fill_value;
    volatile u32 *tilemap;
    u32 tile_pair;
    u32 row;

    /*
     * This is the power-of-two signed-division correction used by the
     * reference before wrapping each coordinate into the 32x32 map.
     */
    if (cell_x < 0) {
        cell_x += 0x1FFFFF;
    }
    if (cell_y < 0) {
        cell_y += 0x1FFFFF;
    }
    cell_x = (cell_x >> 21) & 0x1F;
    cell_y = (cell_y >> 21) & 0x1F;

    map_entry = ((u32 *)0x02020000)[cell_x + (cell_y << 5)];
    special = ((map_entry << 1) >> 25) == 0x15;
    assets = &asset_sets[special];
    state->assets = assets;

    saved_backdrop = *(volatile s16 *)0x05000000;
    Func_080053e8(Func_08002f40(assets->palette), palette_buffer);
    palette_buffer[0] = saved_backdrop;
    dma->source = palette_buffer;
    dma->destination = (void *)0x05000000;
    dma->control = 0x84000070;

    Func_08005340(
        Func_08002f40(assets->tiles_0),
        (void *)0x02038000);
    dma->source = (const void *)0x02038000;
    dma->destination = (void *)0x06008000;
    dma->control = 0x84000800;

    Func_08005340(
        Func_08002f40(assets->tiles_1),
        (void *)0x0203A000);
    dma->source = (const void *)0x0203A000;
    dma->destination = (void *)0x0600A000;
    dma->control = 0x84000800;

    Func_08005340(
        Func_08002f40(assets->tiles_2),
        (void *)0x0203C000);
    dma->source = (const void *)0x0203C000;
    dma->destination = (void *)0x0600C000;
    dma->control = 0x84000800;

    Func_08005340(
        Func_08002f40(assets->tiles_3),
        (void *)0x0203E000);
    dma->source = (const void *)0x0203E000;
    dma->destination = (void *)0x0600E000;
    dma->control = 0x84000800;

    Func_08005340(
        Func_08002f40(assets->map),
        (void *)0x02028000);

    fill_value = 0xF07FF07F;
    dma->source = &fill_value;
    dma->destination = (void *)0x06002800;
    dma->control = 0x85000180;

    tilemap = (volatile u32 *)0x06003000;
    tile_pair = 0x01A901A8;
    for (row = 0; row < 20; row++) {
        u32 column;

        for (column = 0; column < 15; column++) {
            *tilemap++ = tile_pair;
            tile_pair += 0x00020002;
        }
        tilemap++;
    }

    if (special != 0) {
        state->first_special_tiles[0] = 0x10A;
        state->first_special_tiles[1] = 0x10B;
        state->first_special_tiles[2] = 0x10C;
        state->second_special_tiles[0] = 0x11A;
        state->second_special_tiles[1] = 0x11B;
        state->second_special_tiles[2] = 0x11C;
        Func_080113e4();
    }

    Func_08002df0(palette_buffer);
}
