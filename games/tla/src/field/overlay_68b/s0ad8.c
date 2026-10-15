#include "types.h"
#include "dma.h"
#include "io_reg.h"

extern u8 Data_02002590;
extern s16 Data_020025a0[2][160];

// Puts the per-line BG2 horizontal scroll on an HBlank DMA, as overlay 674's Func_02001cac does for BG1.
void Func_02000ad8(void)
{
    s16 *lines = Data_020025a0[Data_02002590];
    s32 first = lines[0];
    volatile u16 *scroll;
    volatile u16 *dma;

    /* FAKEMATCH: keeps the first line's load before the register address is built */
    __asm__("" : : "r"(first));
    scroll = (volatile u16 *)0x04000018;
    *scroll = first;
    dma = REG_DMA0;

    /* FAKEMATCH: the one-pass block reads the control before its mask is started; without it the mask is started first (differ 10) */
    do {
        dma[5] &= 0xc5ff; // Clear the start timing and repeat
        dma[5] &= 0x7fff; // Stop
        lines++;
        /* FAKEMATCH: keeps the cursor step before the last control read (differ 2 without it) */
        __asm__("" : : "r"(lines));
    } while (0);
    (void)dma[5];
    Dma_Set(lines, (void *)scroll, 0xa2600001, (volatile u32 *)dma);
}
