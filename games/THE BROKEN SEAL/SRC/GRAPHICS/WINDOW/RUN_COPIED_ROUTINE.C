#include "DMA.H"
#include "SYSTEM.H"
#include "HEAP_STATE.H"

extern const u8 Tile_Decompress4bpp[];

extern u8 Tile_Decompress4bppCodeSize[];

void *Runtime_AllocateHeapBlock(s32 kind, s32 size);

/* Copies the 4bpp decompressor (DECOMPRESS_4BPP.S) to a heap block and
   runs the routine installed at 0x03001f14 on the work's stream. */
void Ui_RunCopiedRoutine(u8 *work)
{
    u32 size;
    void *code;

    /* FAKEMATCH: the do-while keeps the size load after the arguments. */
    do { size = (u32)Tile_Decompress4bppCodeSize; } while (0);
    code = Runtime_AllocateHeapBlock(49, size);
    Dma_Set((const void *)Tile_Decompress4bpp, code, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    ((void (*)(s32, u8 *))((union HeapState *)gWorkSlot)->slots[49])(
        *(s32 *)(work + 0x604), work);
    Runtime_ReleaseHeapBlock(49);
}
