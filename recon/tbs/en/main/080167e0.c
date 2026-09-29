/* Draft, not exact: 136 of 136 bytes. The reference keeps the DMA control
   halves in fp and r9, the fill offset in sl and the row pointer on the
   stack, and reloads the fill routine into r3 for each row.
   2026-09-29 (Venus): the permuter's temporaries and register hints are
   gone; this plain loop emits the same length and differs only in that
   hoisting (34 diff lines, as before). Indexed pointers (n * 32) hoist
   0x84000000 into fp as the reference does but put the counter on the
   stack; no mix of indexed and stepped pointers, loop direction or
   operand order closes it. */
#include "DMA.H"
#include "IWRAM_CALL.H"

void UiWork_ShiftPanelRowsLeft(s32 step)
{
    s32 shift = step * 6;
    u32 *destination = (u32 *)0x06002520;
    u32 *source = destination + shift;
    u32 *row = (u32 *)0x06002500;
    s32 n;

    for (n = 29; n >= 0; n--) {
        Dma_Set(source, destination, 0x84000000 | (24 - shift), (volatile u32 *)0x040000d4);
        Iwram_FillWords(row + (32 - shift), shift * 4, 0);
        source += 32;
        destination += 32;
        row += 32;
    }
}
