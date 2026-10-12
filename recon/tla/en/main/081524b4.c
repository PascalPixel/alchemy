/* Near miss: differ 2 of 29, the same case as Func_08152474 (see its header): the native
 * starts the destination before the dummy read of the control word. */
#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"

void Func_081524b4(void)
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
    Dma_Set(work + 0x7080, (void *)0x04000014, 0xa6600001, (volatile u32 *)channel);
}
