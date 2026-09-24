/* Draft, not exact (2026-09-24): 21 differing halfwords, 260 of 260 bytes.
   Written from the listing after splitting Map_InitializePerspectiveScene
   (main:080109e8) out of this listing. The bottom-half table goes through a
   pointer local so it reloads per tile as in the ROM. Remaining: that load
   lands in r7 before the top store (ROM: r2 after it), and the column and
   row masks use r7 and r1 where the ROM uses r1 twice. */
#include "DMA.H"

struct MapLayerWork {
    u8 unknown_000[0x110];
    u32 *cell_graphics;
    u8 unknown_114[0x338 - 0x114];
    u16 cells[8];
};

struct Halves {
    u16 top;
    u16 bottom;
};

extern struct MapLayerWork *Data_03001e70;

void *Runtime_AllocateHeapBlock(s32 id, s32 size);
void Resource_DecodeByteLz(const void *source, void *destination);
void Runtime_ReleaseHeapBlock(s32 id);

s32 Map_WriteLayerCellTile(s32 layer, s32 column, s32 row, u32 tile, s32 force)
{
    struct MapLayerWork *work = Data_03001e70;
    u32 *graphics = work->cell_graphics;
    u16 *cell;
    u16 *buffer;
    u16 *source;
    u8 *destination;
    u16 *output;
    u32 i;
    u32 j;
    u32 id;
    u16 *bottom;

    column &= 1;
    row &= 1;
    cell = &work->cells[(layer * 2 + row) * 2 + column];
    if (force == 0 && tile == *cell)
        return 0;
    *cell = tile;
    buffer = Runtime_AllocateHeapBlock(14, 0x400);
    Resource_DecodeByteLz((u8 *)graphics + graphics[tile], buffer);

    source = buffer;
    destination = (u8 *)0x02020000 + (((layer * 2 + row) * 32 + column) << 6);
    for (i = 0; i < 16; i++) {
        Dma_Set(source, destination, 0x84000010, (volatile u32 *)0x040000d4);
        source += 32;
        destination += 128;
    }

    if (force != 0) {
        output = (u16 *)(0x06004000 + ((((layer * 2 + row) << 6) + column) << 5));
        source = buffer;
        bottom = (u16 *)0x02010002;
        for (i = 0; i < 16; i++) {
            for (j = 0; j < 16; j++) {
                id = *source;
                output[0] = ((u16 *)0x02010000)[id * 2];
                output[32] = bottom[id * 2];
                output++;
                source += 2;
            }
            output += 48;
        }
    }
    Runtime_ReleaseHeapBlock(14);
    return 1;
}
