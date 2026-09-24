#include "DMA.H"

#define BattlePresentation_InitializeTilePattern Func_080b5ad4

/* Copies the eight-word tile pattern one row down in VRAM, then hands the
   copy at 0x0600028c to the IWRAM tile routine. */
s32 BattlePresentation_InitializeTilePattern(void)
{
    Dma_Set((const void *)0x06000290, (void *)0x06000280, 0x80000008,
            (volatile u32 *)0x040000d4);
    return ((s32 (*)(void *, s32))0x03000164)((void *)0x0600028c, 20);
}
