/* Draft, not exact: 136 of 136 bytes, 37 differing halfwords / 34 edits.
   Fresh scoring shows the fill routine is hoisted into fp, not reloaded
   per row as the old header claimed; the reference keeps DMA control in fp
   and reloads the routine into r3. The row pointer and fill size therefore
   also get different saved registers/spills. The exact item-menu pattern
   (IwramIrqMain + 0x168) emits identical bytes, as does changing the fill
   result to void. Stop that call-address axis without new loop evidence. */
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
