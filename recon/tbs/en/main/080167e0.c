/* Draft, not exact: 136 of 136 bytes, score 320 (3 register-only, 5
   reordered), was 1160.
   2026-10-01 (matcher 3): a seven-minute permute from the plain loop
   reached this body; its one load-bearing change is the temporary shared by
   the fill size and the loop's exit flag (tmp = shift * 4 ... tmp = n < 0).
   The same body with a separate size, an `if (--n < 0) break`, a do/while,
   shift << 2 for the size or row + 32 - shift for the fill address scores
   1050-1470; the register keywords, 4 * shift and row + (32 - shift) are
   neutral. Remaining: the reference gives the size (step * 24) r8 and the
   fill offset ((32 - shift) * 4) sl, and stores the row pointer to the
   stack after computing source; here the offset outranks the size (the
   greg dump has pseudo 52, size, 3 refs over 21 insns, below 56, offset,
   3 refs over 17), so they swap, and the row goes to the stack first. A
   byte-pointer source = destination + size (to give the size a fourth
   reference) hoists the fill routine into fp instead (1360). A six-minute
   permute from here found nothing lower.
   Earlier (2026-09-29, Venus): the reference keeps the DMA control halves
   in fp and r9, the fill offset in sl and the row pointer on the stack, and
   reloads the fill routine into r3 for each row. */
#include "DMA.H"
#include "IWRAM_CALL.H"

void UiWork_ShiftPanelRowsLeft(s32 step)
{
    u32 *row = (u32 *)0x06002500;
    s32 shift = 6 * step;
    u32 *destination = (u32 *)0x06002520;
    u32 *source = destination + shift;
    s32 n;

    n = 29;
    if (n >= 0) {
        while (1) {
            s32 tmp;
            Dma_Set(source, destination, 0x84000000 | (24 - shift), (volatile u32 *)0x040000d4);
            destination += 32;
            tmp = shift * 4;
            Iwram_FillWords(&row[32 - shift], tmp, 0);
            source += 32;
            row += 32;
            n--;
            tmp = n < 0;
            if (tmp)
                break;
        }
    }
}
