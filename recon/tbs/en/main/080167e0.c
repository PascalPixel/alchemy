/* Draft, not exact (2026-09-24): 37 differing halfwords, 136 of 136 bytes.
   The IWRAM fill now goes through an inline wrapper, so the routine is
   reloaded per row and 0x84000000 is held in fp as in the ROM. Residual:
   the reference computes step * 3 once and keeps shift * 4 in r8 for both
   the source start and the fill size, (32 - shift) * 4 in sl and
   24 - shift in r9; here the loop invariants take other high registers. */
#include "DMA.H"

typedef s32 (*FillWordsFn)(void *dst, s32 size, s32 value);

static __inline__ void FillWords(void *dst, s32 size, s32 value)
{
    ((FillWordsFn)0x03000168)(dst, size, value);
}

void UiWork_ShiftPanelRowsLeft(s32 step)
{
    u32 *row;
    u32 *destination;
    u32 *source;
    s32 n;
    s32 shift;
    s32 size;

    shift = step * 6;
    size = shift * 4;
    destination = (u32 *)0x06002520;
    source = (u32 *)((u8 *)destination + size);
    row = (u32 *)0x06002500;
    for (n = 29; n >= 0; n--) {
        Dma_Set(source, destination, 0x84000000 | (24 - shift), (volatile u32 *)0x040000d4);
        FillWords(row + (32 - shift), size, 0);
        row += 32;
        destination += 32;
        source += 32;
    }
}
