/* Draft of overlay 68B's Func_02000ad8, differ 2: the native steps the line cursor
 * (adds r0, #2) before the last control read; here the read comes first. Combine
 * folds the step into Dma_Set's r0 copy after the read, and sched2 then prefers the
 * read, which has more dependents (the control word's ldr r2 reuses its register).
 * Trials: two empty asm barriers taking first and the stepped cursor as inputs, with
 * a one-pass block around the two masks and the step (overlay 674's Func_02001cac
 * form), match all twelve editions, but S2's barrier count may only fall. Without the
 * one-pass block, differ 10; one-pass blocks around the masks, the step or the read,
 * *lines++, a cursor computed before the load, the read inside Dma_Set's control
 * argument and a control local all keep this swap (differ 2-4); alchemy permute,
 * 8 minutes and 163,486 candidates, best differ 2. */
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
    volatile u16 *dma;

    /* FAKEMATCH: the one-pass block builds BG2HOFS's address after the first line is loaded (differ 10 without it) */
    do {
        REG_BG2HOFS = first;
        dma = REG_DMA0;
    } while (0);
    dma[5] &= 0xc5ff; // Clear the start timing and repeat
    dma[5] &= 0x7fff; // Stop
    (void)dma[5];
    Dma_Set(lines + 1, (void *)&REG_BG2HOFS, 0xa2600001, (volatile u32 *)dma);
}
