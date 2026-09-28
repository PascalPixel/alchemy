#include "DMA.H"

extern const u8 System_BasicColorPalette[];
void PaletteDma_LoadBlock(void)
{
    Dma_Set((const void *)System_BasicColorPalette, (void *)0x05000200, 0x800000e0, (volatile u32 *)0x040000d4);
}
