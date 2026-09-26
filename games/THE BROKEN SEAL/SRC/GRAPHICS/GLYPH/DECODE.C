#include "DMA.H"

u8 *Runtime_AllocateHeapBlock(s32 slot, u32 size);
void Runtime_ReleaseHeapBlock(s32 slot);

/* Heap block addresses, indexed by block number. */
extern void *Data_03001e50[];

/* ARM routines copied into heap block 49 before each call; their lengths
   are link-time symbols. */
extern u8 Data_00000278[];
extern u8 Data_0000009c[];
extern u8 Data_0000007c[];

#define ROUTINE_BLOCK 49

void UiGlyph_DecodeWithHeapRoutines(u8 *glyph, s32 outlined)
{
    void *code;

    {
        u32 size;

        /* FAKEMATCH: retain the size load after saving both arguments. */
        do {
            size = (u32)Data_00000278;
        } while (0);
        code = Runtime_AllocateHeapBlock(ROUTINE_BLOCK, size);
        size >>= 2;
        Dma_Set((const void *)0x08015afc, code,
            0x84000000 | size, (volatile u32 *)0x040000d4);
    }
    ((void (*)(const void *, u8 *))Data_03001e50[ROUTINE_BLOCK])(
        *(const void **)(glyph + 0x604), glyph);
    Runtime_ReleaseHeapBlock(ROUTINE_BLOCK);
    if (outlined) {
        u32 size;

        size = (u32)Data_0000009c;
        code = Runtime_AllocateHeapBlock(ROUTINE_BLOCK, size);
        size >>= 2;
        Dma_Set((const void *)0x08015d74, code,
            0x84000000 | size, (volatile u32 *)0x040000d4);
    } else {
        u32 size;

        size = (u32)Data_0000007c;
        code = Runtime_AllocateHeapBlock(ROUTINE_BLOCK, size);
        size >>= 2;
        Dma_Set((const void *)0x08015e10, code,
            0x84000000 | size, (volatile u32 *)0x040000d4);
    }
    ((void (*)(u8 *, u8 *, u32, u32))Data_03001e50[ROUTINE_BLOCK])(
        glyph, glyph + 0x400, *(u16 *)(glyph + 0x600), *(u16 *)(glyph + 0x602));
    Runtime_ReleaseHeapBlock(ROUTINE_BLOCK);
}
