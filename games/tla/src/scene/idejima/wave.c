// The water's wave scroll: each scanline's vertical offset for BG3 from the
// back one of two line tables, streamed by DMA 0 at every HBlank.
#include "types.h"
#include "io_reg.h"
#include "dma.h"

extern s16 gData_02003c14;
extern s16 gData_02003990[2][160];

void Func_02001558(void)
{
    s16 *line = gData_02003990[(s16)(gData_02003c14 ^ 1)];

    s32 offset = line[0];

    /* FAKEMATCH: a one-pass block builds BG3VOFS's address after the first line is loaded (one reordered row). */
    do
    {
    } while (0);
    *(volatile u16 *)0x0400001e = offset;
    Dma_Set(line + 1, (void *)0x0400001e, 0xa2600001, REG_DMA0);
}
