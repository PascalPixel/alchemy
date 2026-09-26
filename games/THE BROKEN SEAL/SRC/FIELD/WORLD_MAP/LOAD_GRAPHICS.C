#include "TYPES.H"
#include "DMA.H"

u32 Runtime_BumpAllocate(s32 size);
void Runtime_BumpFree(void *block);
u32 Resource_GetTableEntry(u32 index);
s32 Resource_DecodeByteLz(const void *source, void *destination);
s32 Resource_DecodeType01(const void *source, void *destination);
void Map_UpdateCurrentTileBlock(void);

extern u8 *Data_03001e70;
extern u32 Data_080132cc[][6];

struct WorldCell {
    u32 tile : 24;
    u32 kind : 7;
    u32 flag : 1;
};

#define BG_PALETTE ((s16 *)0x05000000)
#define WORLD_CELLS ((struct WorldCell *)0x02020000)

struct WorldMapState {
    u8 unk_000[0x11c];
    u32 *resources;
    u8 unk_120[0x2c];
    u16 animated_a[3];
    u8 unk_152[0x1a];
    u16 animated_b[3];
};

/* Loads the world map graphics for the region around (x, z): the palette
   (keeping the backdrop colour), four tile banks and the cell graphics,
   clears the overlay map, lays out the fixed tile grid, and for the sea
   variant also sets up its animated tiles. */
void WorldMap_LoadGraphics(s32 x, s32 z)
{
    struct WorldMapState *state;
    u32 *resources;
    u8 *buffer;
    u32 *cursor;
    u32 tile;
    s32 variant;
    s32 row;
    s32 col;
    s16 value;
    volatile u32 fill;

    variant = 0;
    buffer = (u8 *)Runtime_BumpAllocate(0x200);
    state = (struct WorldMapState *)Data_03001e70;
    if (WORLD_CELLS[((x / 0x200000) & 31) + (((z / 0x200000) & 31) << 5)].kind == 21)
        variant = 1;
    resources = Data_080132cc[variant];
    state->resources = resources;
    value = BG_PALETTE[0];
    Resource_DecodeByteLz((const void *)Resource_GetTableEntry(resources[0]), buffer);
    *(s16 *)buffer = value;
    Dma_Set(buffer, BG_PALETTE, 0x84000070, (volatile u32 *)0x040000d4);
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[1]), (void *)0x02038000);
    Dma_Set((void *)0x02038000, (void *)0x06008000, 0x84000800, (volatile u32 *)0x040000d4);
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[2]), (void *)0x0203a000);
    Dma_Set((void *)0x0203a000, (void *)0x0600a000, 0x84000800, (volatile u32 *)0x040000d4);
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[3]), (void *)0x0203c000);
    Dma_Set((void *)0x0203c000, (void *)0x0600c000, 0x84000800, (volatile u32 *)0x040000d4);
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[4]), (void *)0x0203e000);
    Dma_Set((void *)0x0203e000, (void *)0x0600e000, 0x84000800, (volatile u32 *)0x040000d4);
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[5]), (void *)0x02028000);
    fill = 0xf07ff07f;
    Dma_Set((const void *)&fill, (void *)0x06002800, 0x85000180, (volatile u32 *)0x040000d4);
    cursor = (u32 *)0x06003000;
    tile = 0x01a901a8;
    for (row = 0; row < 20; row++) {
        for (col = 0; col < 15; col++) {
            *cursor++ = tile;
            tile += 0x00020002;
        }
        cursor++;
    }
    if (variant == 1) {
        state->animated_a[0] = 0x10a;
        state->animated_a[1] = 0x10b;
        state->animated_a[2] = 0x10c;
        state->animated_b[0] = 0x11a;
        state->animated_b[1] = 0x11b;
        state->animated_b[2] = 0x11c;
        Map_UpdateCurrentTileBlock();
    }
    Runtime_BumpFree(buffer);
}
