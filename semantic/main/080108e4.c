typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct Dma_080108e4 {
    const void *source;
    void *destination;
    u32 control;
};

void *Func_080048b0(s32, s32);
void Func_080053e8(const void *, void *);
void Func_08002dd8(s32);

/*
 * Load one parity-banked metatile sheet. The decompressed 16x16 index grid is
 * copied into the EWRAM graphics banks and, on a forced refresh, expanded
 * through the tile-definition table into the paired VRAM tilemap rows.
 */
s32 Func_080108e4(
    s32 layer,
    s32 tile_y,
    s32 tile_x,
    u16 tile_id,
    s32 force_refresh
) {
    u8 *engine;
    u8 *tileset;
    u16 *cached_id;
    u16 *indices;
    u16 *source;
    u16 *tilemap;
    const u16 *tile_definitions;
    struct Dma_080108e4 *dma3;
    u32 tileset_offset;
    s32 x_parity;
    s32 y_parity;
    s32 bank;
    s32 row;
    s32 column;

    engine = *(u8 **)0x03001E70;
    tileset = *(u8 **)(engine + 0x110);
    y_parity = tile_y & 1;
    x_parity = tile_x & 1;
    bank = (layer * 2 + x_parity) * 2 + y_parity;
    cached_id = (u16 *)(engine + 0x338) + bank;

    if (!force_refresh && *cached_id == tile_id) {
        return 0;
    }
    *cached_id = tile_id;

    indices = Func_080048b0(0xE, 0x400);
    tileset_offset = *(u32 *)(tileset + tile_id * 4);
    Func_080053e8(tileset + tileset_offset, indices);

    dma3 = (struct Dma_080108e4 *)0x040000D4;
    source = indices;
    for (row = 0; row < 16; row++) {
        dma3->source = source;
        dma3->destination =
            (u8 *)0x02020000 +
            ((layer * 2 + x_parity) * 32 + y_parity) * 64 +
            row * 0x80;
        dma3->control = 0x84000010;
        source += 0x20;
    }

    if (force_refresh) {
        tilemap =
            (u16 *)((u8 *)0x06004000 +
                    ((layer * 2 + x_parity) * 64 + y_parity) * 32);
        tile_definitions = (const u16 *)0x02010000;
        source = indices;
        for (row = 0; row < 16; row++) {
            for (column = 0; column < 16; column++) {
                u32 definition = *source;
                tilemap[0] = tile_definitions[definition * 2];
                tilemap[0x20] = tile_definitions[definition * 2 + 1];
                tilemap++;
                source += 2;
            }
            tilemap += 0x30;
        }
    }

    Func_08002dd8(0xE);
    return 1;
}
