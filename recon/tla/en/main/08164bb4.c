/* Draft, differ 2 of 29: The Broken Seal's BattleFx_ArmPaletteHBlankDma
 * with The Lost Age's sheet offset. The one-pass block reads the control
 * word before its mask is started (differ 4 without it). Left: the native
 * starts the destination's 0x05000000 (movs r1, #160) before the last
 * control read and shifts it after; this starts it after the read. A
 * temporary for the destination is folded back into the call. */
#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"

void Func_08164bb4(void)
{
    u8 *work = Ram_HeapSlots->battle_fx_work[0];
    volatile u16 *channel = REG_DMA0;

    /* FAKEMATCH: the one-pass block reads the control word before its mask is started, as overlay 674's s1cac.c (differ 4 without it). */
    do {
        channel[5] &= 0xc5ff;
        channel[5] &= 0x7fff;
    } while (0);
    (void)channel[5];
    Dma_Set(work + 0x7640, (void *)0x05000000, 0xa2600001, (volatile u32 *)channel);
}
