/* Draft, not exact: 48 differing halfwords, 136-byte candidate for the
   136-byte owner (2026-09-23). Residual: register allocation of the loop
   invariants: the reference keeps the countdown in r7, the byte size in r8,
   the word count in r9, the DMA control in r11 and the tail offset in r10
   with the row pointer spilled to sp; the countdown lands in r8 here. */

#include "DMA.H"

typedef s32 (*FillWordsFn)(void *dst, s32 size, s32 value);

/* Shifts the 24-word panel of each of 30 map rows left by three entries per
   step and clears the uncovered tail of each row. */
void UiWork_ShiftPanelRowsLeft(s32 step)
{
    u8 *row;
    u8 *destination;
    u8 *source;
    s32 n;

    destination = (u8 *)0x06002520;
    source = destination + step * 24;
    row = (u8 *)0x06002500;
    n = 29;
    do {
        Dma_Set(source, destination, 0x84000000 | (24 - step * 6), (volatile u32 *)0x040000d4);
        ((FillWordsFn)0x03000168)(row + (32 - step * 6) * 4, step * 24, 0);
        row += 128;
        destination += 128;
        source += 128;
        n--;
    } while (n >= 0);
}
