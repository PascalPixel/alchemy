/* Draft, not exact (2026-09-24): 38 differing halfwords, 136 of 136 bytes.
   The shift is now one step * 6 word count, as the ROM computes it
   (3 * step, then << 1 and << 3), and the row pointer stays in its stack
   slot. Residual: loop.c hoists the IWRAM fill routine's address out of
   the loop and rebuilds 0x84000000 inside it; the reference does the
   opposite (0x84000000 in fp, the routine reloaded into r3 per row). */

#include "DMA.H"

typedef s32 (*FillWordsFn)(void *dst, s32 size, s32 value);

void UiWork_ShiftPanelRowsLeft(s32 step)
{
    u32 *row;
    u32 *destination;
    u32 *source;
    s32 n;
    s32 shift;

    shift = step * 6;
    destination = (u32 *)0x06002520;
    source = destination + shift;
    row = (u32 *)0x06002500;
    for (n = 29; n >= 0; n--) {
        Dma_Set(source, destination, 0x84000000 | (24 - shift), (volatile u32 *)0x040000d4);
        ((FillWordsFn)0x03000168)(row + (32 - shift), shift * 4, 0);
        row += 32;
        destination += 32;
        source += 32;
    }
}
