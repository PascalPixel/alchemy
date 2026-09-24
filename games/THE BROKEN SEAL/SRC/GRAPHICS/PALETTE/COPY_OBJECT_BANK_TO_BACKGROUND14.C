#include "DMA.H"

#define Palette_CopyObjectBankToBackground14 Func_080a22f4

/* Copies object palette bank 0 over background bank 14, then copies object
   colour 4 of bank 0 into background bank 14's entry 14. */
void Palette_CopyObjectBankToBackground14(void)
{
    Dma_Set((const void *)0x05000200, (void *)0x050001c0, 0x80000010, (volatile u32 *)0x040000d4);
    Dma_Set((const void *)0x050001e8, (void *)0x050001dc, 0x80000001, (volatile u32 *)0x040000d4);
}
