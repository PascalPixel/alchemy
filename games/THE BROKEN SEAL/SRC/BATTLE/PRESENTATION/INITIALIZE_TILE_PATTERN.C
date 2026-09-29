#include "DMA.H"
#include "IWRAM_CALL.H"


/* Copies the eight-word tile pattern one row down in VRAM, then clears the
   five words at 0x0600028c with the IWRAM word clear. */
s32 BattlePresentation_InitializeTilePattern(void)
{
    Dma_Set((const void *)0x06000290, (void *)0x06000280, 0x80000008,
            (volatile u32 *)0x040000d4);
    return Iwram_ClearWords((void *)0x0600028c, 20);
}
