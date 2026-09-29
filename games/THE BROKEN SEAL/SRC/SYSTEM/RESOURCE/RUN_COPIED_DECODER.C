#include "DMA.H"
#include "SYSTEM.H"
#include "MAP.H"

extern const u8 TileMap_DrawRows[];

struct RuntimeCells {
    u8 unknown_000[196];
    void (*decode)(s32, s32, void *, void *);
};

extern struct RuntimeCells gWorkSlot;
extern u8 Value_0000027c[];
extern u8 gDecodeBuffer[];

void *Runtime_AllocateHeapBlock(s32 kind, s32 size);

/* Copies the ARM decoder TileMap_DrawRows (Value_0000027c bytes) to a heap
   block and runs the decoder gWorkSlot holds on a and b. */
void Resource_RunCopiedDecoder(s32 a, s32 b)
{
    u8 *base;
    u32 size;
    void *code;

    base = gDecodeBuffer;
    /* FAKEMATCH: the do-whiles order the size load and the call. */
    do { size = (u32)Value_0000027c; } while (0);
    code = Runtime_AllocateHeapBlock(49, size);
    Dma_Set((const void *)TileMap_DrawRows, code, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    do { gWorkSlot.decode(a, b, gBgTileBuffer + 0x4000, base + 0x1000); } while (0);
    Runtime_ReleaseHeapBlock(49);
}
