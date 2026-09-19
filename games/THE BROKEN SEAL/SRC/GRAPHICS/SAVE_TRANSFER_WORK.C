#include "DMA.H"
void Graphics_SaveTransferWork(void *destination)
{
    Dma_Set((const void *)0x03000350, destination, 0x8400000c, (volatile u32 *)0x040000d4);
}
