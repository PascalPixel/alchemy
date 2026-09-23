/* Draft, not exact: 93 differing halfwords, 204-byte candidate for the
   200-byte owner (2026-09-23). Residual: with the size load wrapped
   (FAKEMATCH) the entry matches; the parameter and first code size swap r5
   and r6, and the shared 0x84000000 is built in r3 and kept in r8 where the
   reference builds it in r2; one Dma_Set after an if/else choosing code and
   size is worse (98). */

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
    u32 size;

    do {
        size = (u32)Data_00000278;
    } while (0);
    Dma_Set((const void *)0x08015afc, Runtime_AllocateHeapBlock(ROUTINE_BLOCK, size),
        0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    ((void (*)(const void *, u8 *))Data_03001e50[ROUTINE_BLOCK])(*(const void **)(glyph + 0x604), glyph);
    Runtime_ReleaseHeapBlock(ROUTINE_BLOCK);
    if (outlined) {
        size = (u32)Data_0000009c;
        Dma_Set((const void *)0x08015d74, Runtime_AllocateHeapBlock(ROUTINE_BLOCK, size),
            0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    } else {
        size = (u32)Data_0000007c;
        Dma_Set((const void *)0x08015e10, Runtime_AllocateHeapBlock(ROUTINE_BLOCK, size),
            0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    }
    ((void (*)(u8 *, u8 *, u32, u32))Data_03001e50[ROUTINE_BLOCK])(glyph, glyph + 0x400,
        *(u16 *)(glyph + 0x600), *(u16 *)(glyph + 0x602));
    Runtime_ReleaseHeapBlock(ROUTINE_BLOCK);
}
