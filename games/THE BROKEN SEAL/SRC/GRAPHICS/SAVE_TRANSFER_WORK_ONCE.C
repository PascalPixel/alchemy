#include "DMA.H"
void Graphics_SaveTransferWorkOnce(void)
{
    if (*(s32 *)0x03001cc4 <= 0) {
        Dma_Set((void *)0x03000350, *(void **)0x03001d2c, 0x8400000c, (volatile u32 *)0x040000d4);
        (*(s32 *)0x03001cc4)++;
        *(u8 **)0x03001d2c += 48;
    }
}
