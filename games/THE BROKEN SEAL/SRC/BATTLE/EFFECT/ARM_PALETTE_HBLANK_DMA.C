#include "DMA.H"

/* H-blank callback: feed the per-line backdrop colours in the battle work
   area to palette entry 0. */
void BattleFx_ArmPaletteHBlankDma(void)
{
    u8 *work = *(u8 **)0x03001eec;
    volatile u16 *channel = (volatile u16 *)0x040000b0;
    channel[5] &= 0xc5ff;
    channel[5] &= 0x7fff;
    (void)channel[5];
    Dma_Set(work + 0x1f80, (void *)0x05000000, 0xa2600001, (volatile u32 *)channel);
}
