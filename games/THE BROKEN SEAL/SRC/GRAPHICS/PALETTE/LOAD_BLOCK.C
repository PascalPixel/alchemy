#include "DMA.H"
void PaletteDma_LoadBlock(void)
{
    Dma_Set((const void *)0x0800779c, (void *)0x05000200, 0x800000e0, (volatile u32 *)0x040000d4);
}
