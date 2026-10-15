#include "types.h"
#include "dma.h"
#include "frame.h"
#include "io_reg.h"

extern u8 gData_02002590;
extern s16 gData_020025a0[2][160];

// Every fourth frame rotates colours 1-14 of background palettes 7 and 8 by one place.
// The loops are written with goto, as overlay 689's Func_020002f8: the bytes rebuild
// the palette address on every pass.
void Func_02000a6c(void)
{
    if ((gFrameCount & 3) == 0) {
        volatile u16 *palette;
        volatile u16 *slot;
        s16 saved;
        s32 index;

        saved = *(volatile u16 *)0x050000fc;
        index = 14;
    first:
        palette = (volatile u16 *)0x050000e0;
        palette[index] = (palette - 1)[index];
        index--;
        if (index != 1)
            goto first;
        slot = (volatile u16 *)0x050000e2;
        *slot = saved;
        slot += 29;
        saved = *slot;
        index = 14;
    second:
        palette = (volatile u16 *)0x05000100;
        palette[index] = (palette - 1)[index];
        index--;
        if (index != 1)
            goto second;
        *(volatile u16 *)0x05000102 = saved;
    }
}

// Puts the per-line BG2 horizontal scroll on an HBlank DMA, as overlay 674's Func_02001cac does for BG1.
void Func_02000ad8(void)
{
    s16 *lines = gData_020025a0[gData_02002590];
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
