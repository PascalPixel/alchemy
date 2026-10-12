/* Near miss: differ 2 of 29 (one reordered row; differ 6 before the one-pass block and the
 * slot fix). The native starts the destination (movs r1, #128) before the dummy read of the
 * control word and finishes it after the source offset; the draft reads first. Tried: the
 * destination as a local (unchanged), with an empty asm on it (movs first, but lsls r1 and
 * adds r1 follow too early, differ 4), with an input-only asm (all three early, differ 6),
 * a one-pass block on the dummy read (the read first, differ 2).
 * Func_081524b4 is the same function for 0x04000014. */
#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"

void Func_08152474(void)
{
    u8 *work = Ram_HeapSlots->battle_fx_work[0];
    volatile u16 *channel = (volatile u16 *)REG_DMA0;
    u32 control;

    /* FAKEMATCH: the one-pass block ends the scheduling block: the native reads the control word before it builds the mask 0xc5ff (one reordered row). */
    do {
        control = channel[5];
    } while (0);
    control &= 0xc5ff;
    channel[5] = control;
    channel[5] &= 0x7fff;
    (void)channel[5];
    Dma_Set(work + 0x7080, (void *)0x04000028, 0xa6600001, (volatile u32 *)channel);
}
