#include "DMA.H"

extern u8 gDisp[];
void BattlePresentation_UploadTileVariant(void)
{
    u32 variant = **(u32 **)gDisp - 1;
    if (variant <= 31)
        Dma_Set((u8 *)0x080c5a30 + (variant >> 2) * 32, (void *)0x06005000, 0x84000008, (volatile u32 *)0x040000d4);
}
