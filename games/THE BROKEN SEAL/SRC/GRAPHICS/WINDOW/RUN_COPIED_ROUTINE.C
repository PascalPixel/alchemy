#include "DMA.H"
#include "SYSTEM.H"
#include "RUNTIME_MEM.H"
#include "GLYPH.H"
#include "HEAP_STATE.H"

extern const u8 Tile_Decompress4bpp[];

extern u8 Tile_Decompress4bppCodeSize[];


/* Copies the 4bpp decompressor (DECOMPRESS_4BPP.S) to a heap block and
   runs the routine installed at 0x03001f14 on the work's stream. */
void Ui_RunCopiedRoutine(GlyphTransfer *work)
{
    u32 size;
    void *code;

    /* FAKEMATCH: the do-while keeps the size load after the arguments. */
    do { size = (u32)Tile_Decompress4bppCodeSize; } while (0);
    code = Runtime_AllocateHeapBlock(49, size);
    Dma_Set((const void *)Tile_Decompress4bpp, code, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    ((void (*)(s32, u8 *))((union HeapState *)gWorkSlot)->slots[49])(
        (s32)work->encoded, (u8 *)work);
    Runtime_ReleaseHeapBlock(49);
}
