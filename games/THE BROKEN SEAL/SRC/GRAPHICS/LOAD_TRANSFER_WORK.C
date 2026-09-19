#include "DMA.H"
void Graphics_LoadTransferWork(const void *source)
{
    Dma_Set(source, (void *)0x03000350, 0x8400000c, (volatile u32 *)0x040000d4);
}
