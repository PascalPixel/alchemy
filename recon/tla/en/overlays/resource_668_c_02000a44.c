/* Draft, differ 2 of 35: the wave scroll's HBlank DMA for BG1, with ☀️'s DMA 0 stop sequence (BattleFx_ArmWin0HBlankDma). With the channel taken in a one-pass block (alchemy permute) one pair is left: the listing adds 2 to the source line before the stop's dummy read; this draft after it. Tried: line++ or *line++ before the read, the source as a local set first or between the masks and the read. */
#include "types.h"
#include "io_reg.h"
#include "dma.h"

extern u8 Data_020054f0[];
extern u16 Data_020056e0[2][160];

void Func_02000a44(void)
{
    u16 *line = Data_020056e0[Data_020054f0[0] ^ 1];
    volatile u16 *channel;

    *(volatile u16 *)0x04000016 = line[0];
    do {
        channel = REG_DMA0;
    } while (0);
    channel[5] &= 0xc5ff;
    channel[5] &= 0x7fff;
    (void)channel[5];
    Dma_Set(line + 1, (void *)0x04000016, 0xa2600001, (volatile u32 *)channel);
}
