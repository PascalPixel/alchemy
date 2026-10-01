/* NONMATCHING: Map_WriteLayerCellTile, measured 2026-10-01.
 * Source hypothesis: Keep the masked column before the independent row mask through an empty value boundary.
 * EN: 256/260 complete linked bytes, 213 differing byte positions, first +0xc, including the complete literal pool.
 * All six ordinary TBS target objects compile; text extents in bytes:
 * JA 256; EN 256; DE 256; ES 256; FR 256; IT 256.
 * The other five editions have no complete linked proof for this attempt.
 * RAM addresses remain owned by RAM_BUFFER.H; globals use their physical labels.
 * This source-only S4 draft earns no credit; compiler options and
 * production routing are unchanged.
 */
#include "DMA.H"
#include "RAM_BUFFER.H"

struct MapLayerWork {
    u8 unknown_000[0x110];
    u32 *cell_graphics;
    u8 unknown_114[0x338 - 0x114];
    u16 cells[8];
};

extern struct MapLayerWork *gMapWork;
extern u32 gMapBlocks[];
struct Halves {
    u16 top;
    u16 bottom;
};

extern struct Halves gMapCellBuffer[];

void *Runtime_AllocateHeapBlock(s32 id, s32 size);
void Resource_DecodeByteLz(const void *source, void *destination);
void Runtime_ReleaseHeapBlock(s32 id);

s32 Map_WriteLayerCellTile(s32 layer, s32 column, s32 row, u32 tile, s32 force)
{
    struct MapLayerWork *work = gMapWork;
    u32 *graphics = work->cell_graphics;
    u16 *cell;
    u16 *buffer;
    u16 *source;
    u8 *destination;
    u16 *output;
    u32 i;
    u32 j;
    u32 id;
    struct Halves *cells;

    column &= 1;
    /* FAKEMATCH: keep the masked column input before the independent row mask without changing either value. */
    asm("" : "+r"(column), "+r"(row));
    row &= 1;
    cell = &work->cells[(layer * 2 + row) * 2 + column];
    if (force == 0 && tile == *cell)
        return 0;
    *cell = tile;
    buffer = Runtime_AllocateHeapBlock(14, 0x400);
    Resource_DecodeByteLz((u8 *)graphics + graphics[tile], buffer);

    source = buffer;
    destination = (u8 *)gMapBlocks + (((layer * 2 + row) * 32 + column) << 6);
    for (i = 0; i < 16; i++) {
        Dma_Set(source, destination, 0x84000010, (volatile u32 *)0x040000d4);
        source += 32;
        destination += 128;
    }

    if (force != 0) {
        output = (u16 *)(0x06004000 + ((((layer * 2 + row) << 6) + column) << 5));
        source = buffer;
        cells = gMapCellBuffer;
        i = 0;
    row:
        j = 0;
    tile:
        id = *source;
        output[0] = cells[id].top;
        output[32] = *(u16 *)(Ram_MapCellBuffer + 2 + id * 4);
        output++;
        source += 2;
        if (++j < 16)
            goto tile;
        output += 48;
        if (++i < 16)
            goto row;
    }
    Runtime_ReleaseHeapBlock(14);
    return 1;
}
