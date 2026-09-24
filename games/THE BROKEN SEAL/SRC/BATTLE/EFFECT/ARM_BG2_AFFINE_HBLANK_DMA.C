#include "DMA.H"

/* H-blank callback: feed the per-line BG2X/BG2Y words in the battle work
   area to the BG2 affine reference point. */
void BattleFx_ArmBg2AffineHBlankDma(void)
{
    u8 *work = *(u8 **)0x03001eec;
    volatile u16 *channel = (volatile u16 *)0x040000b0;
    channel[5] &= 0xc5ff;
    channel[5] &= 0x7fff;
    (void)channel[5];
    Dma_Set(work + 0x6980, (void *)0x04000028, 0xa6600001, (volatile u32 *)channel);
}
