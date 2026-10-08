#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "irq.h"

void Runtime_InstallIwramAndIrqs(void)
{
    volatile u16 *ime;
    s32 zero;
    s32 value;
    s32 one;

    ime = &REG_IME;
    zero = 0;
    /* FAKEMATCH: the empty do-while places the IME store before the
       IWRAM address load; the direct form scored 2150 and ordinary shared
       locals scored 890 with different loads, registers and ordering. */
    do {
        *ime = zero;
    } while (0);
    Dma_Set((const void *)IwramRuntime_Rom, IwramIrqMain, 0x84000400, REG_DMA3);
    /* FAKEMATCH: the one-pass vector-store boundary scores 695 versus
       795 for a direct store or a boundary around the runtime copy. */
    do {
        gIrqVector = IwramIrqMain;
    } while (0);
    Dma_Set((const void *)Runtime_IrqHandlers, (void *)gIrqHandlers, 0x8400000e, REG_DMA3);
    REG_DISPSTAT = zero;
    value = 0xc3ff;
    REG_KEYCNT = value;
    value = 0x3001;
    REG_IE = value;
    one = 1;
    *ime = one;
}
