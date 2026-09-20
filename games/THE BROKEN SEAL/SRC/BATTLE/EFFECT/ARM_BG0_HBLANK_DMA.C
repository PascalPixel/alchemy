#include "DMA.H"
void BattleFx_ArmBg0HBlankDma(void)
{
    u8 *work = *(u8 **)0x03001ea8;
    if (!work[660]) {
        u32 offset = work[650] * 324;
        volatile u16 *channel = (volatile u16 *)0x040000b0;
        channel[5] &= 0xc5ff;
        channel[5] &= 0x7fff;
        (void)channel[5];
        Dma_Set(work + offset, (void *)0x04000010, 0xa2600001, (volatile u32 *)channel);
    }
}
