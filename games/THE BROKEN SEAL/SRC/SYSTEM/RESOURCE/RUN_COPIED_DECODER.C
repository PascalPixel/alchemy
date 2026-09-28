#include "DMA.H"
#include "SYSTEM.H"

extern const u8 TileMap_DrawRows[];

struct RuntimeCells {
    u8 unknown_000[196];
    void (*decode)(s32, s32, void *, void *);
};

extern struct RuntimeCells gWorkSlot;
extern u8 TileMap_DrawRowsCodeSize[];
extern u8 gDecodeBuffer[];

void *Runtime_AllocateHeapBlock(s32 kind, s32 size);

/* Copies the map-row renderer (DRAW_MAP.S) to a heap block and runs the
   routine installed at 0x03001f14 on a and b. */
void Resource_RunCopiedDecoder(s32 a, s32 b)
{
    u8 *base;
    u32 size;
    void *code;

    base = gDecodeBuffer;
    /* FAKEMATCH: the do-whiles order the size load and the call. */
    do { size = (u32)TileMap_DrawRowsCodeSize; } while (0);
    code = Runtime_AllocateHeapBlock(49, size);
    Dma_Set((const void *)TileMap_DrawRows, code, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    do { gWorkSlot.decode(a, b, (void *)0x0203c000, base + 0x1000); } while (0);
    Runtime_ReleaseHeapBlock(49);
}
