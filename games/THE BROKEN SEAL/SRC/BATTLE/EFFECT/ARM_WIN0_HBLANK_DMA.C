#include "DMA.H"

/* H-blank callback: feed the per-line WIN0H table at 0x02010000 to WIN0H. */
void BattleFx_ArmWin0HBlankDma(void)
{
    volatile u16 *channel = (volatile u16 *)0x040000b0;
    channel[5] &= 0xc5ff;
    channel[5] &= 0x7fff;
    (void)channel[5];
    Dma_Set((void *)0x02010000, (void *)0x04000040, 0xa2600001, (volatile u32 *)channel);
}
