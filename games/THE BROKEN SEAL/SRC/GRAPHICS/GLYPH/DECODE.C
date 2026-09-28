#include "DMA.H"

extern const u8 Func_08015afc[];
extern const u8 Func_08015d74[];
extern const u8 Func_08015e10[];

u8 *Runtime_AllocateHeapBlock(s32 slot, u32 size);
void Runtime_ReleaseHeapBlock(s32 slot);

/* Heap block addresses, indexed by block number. */
extern void *Data_03001e50[];

/* ARM routines copied into heap block 49 before each call. */
extern u8 Tile_Decompress4bppCodeSize[];
extern u8 Tile_ExpandMaskedCodeSize[];
extern u8 Tile_ExpandOpaqueCodeSize[];

#define ROUTINE_BLOCK 49

void UiGlyph_DecodeWithHeapRoutines(u8 *glyph, s32 outlined)
{
    void *code;

    {
        u32 size;

        /* FAKEMATCH: retain the size load after saving both arguments. */
        do {
            size = (u32)Tile_Decompress4bppCodeSize;
        } while (0);
        code = Runtime_AllocateHeapBlock(ROUTINE_BLOCK, size);
        size >>= 2;
        Dma_Set((const void *)Func_08015afc, code,
            0x84000000 | size, (volatile u32 *)0x040000d4);
    }
    ((void (*)(const void *, u8 *))Data_03001e50[ROUTINE_BLOCK])(
        *(const void **)(glyph + 0x604), glyph);
    Runtime_ReleaseHeapBlock(ROUTINE_BLOCK);
    if (outlined) {
        u32 size;

        size = (u32)Tile_ExpandMaskedCodeSize;
        code = Runtime_AllocateHeapBlock(ROUTINE_BLOCK, size);
        size >>= 2;
        Dma_Set((const void *)Func_08015d74, code,
            0x84000000 | size, (volatile u32 *)0x040000d4);
    } else {
        u32 size;

        size = (u32)Tile_ExpandOpaqueCodeSize;
        code = Runtime_AllocateHeapBlock(ROUTINE_BLOCK, size);
        size >>= 2;
        Dma_Set((const void *)Func_08015e10, code,
            0x84000000 | size, (volatile u32 *)0x040000d4);
    }
    ((void (*)(u8 *, u8 *, u32, u32))Data_03001e50[ROUTINE_BLOCK])(
        glyph, glyph + 0x400, *(u16 *)(glyph + 0x600), *(u16 *)(glyph + 0x602));
    Runtime_ReleaseHeapBlock(ROUTINE_BLOCK);
}
