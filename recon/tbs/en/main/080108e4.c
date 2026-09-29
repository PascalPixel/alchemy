/* Draft, not exact: Map_WriteLayerCellTile, main:080108e4, complete
   260-byte owner [080108e4, 080109e8).
   2026-09-29: the buffers are named now. 0x02020000 is gMapBlocks and
   0x02010000 is gMapCellBuffer, read as top/bottom halfword pairs, and the
   work is gMapWork. This spelling scores 320 (4 register-only, 1 inserted,
   2 deleted): the prologue and the tile copy match, and what remains is the
   pair loop. The reference adds a separately loaded gMapCellBuffer + 2 to
   the scaled id inside the loop; here CSE derives the bottom address from
   the hoisted table base ([base + id * 4, #2]). Every spelling of the
   bottom access that names the symbol (a bottom pointer local, byte
   offsets, a u16 view, an address of the field) scores 1195-1255 instead,
   because loop.c hoists gMapCellBuffer + 2 as well. The earlier draft
   reached 300 only by spelling both buffers as literal EWRAM addresses,
   which stand in for these symbols and cannot be adopted.
   Earlier notes: the bottom-half table through a pointer local reloads per
   tile as in the ROM; a scoped CopyTile helper fixes both mask roles but
   merges the two table constants (260 bytes / 53 halfwords, 22 edits);
   separate named table symbols hoist both bases (264 / 58, 30 edits). */
#include "DMA.H"

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

    column &= 1;
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
        for (i = 0; i < 16; i++) {
            for (j = 0; j < 16; j++) {
                id = *source;
                output[0] = gMapCellBuffer[id].top;
                output[32] = gMapCellBuffer[id].bottom;
                output++;
                source += 2;
            }
            output += 48;
        }
    }
    Runtime_ReleaseHeapBlock(14);
    return 1;
}
