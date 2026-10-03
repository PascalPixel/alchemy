#include "TYPES.H"
#include "DMA.H"
#include "IRQ.H"
#include "IO_REG.H"

/* Copies the IWRAM runtime and its IRQ dispatch table into place with
   interrupts off, points the interrupt vector at it, then enables the
   V-blank and keypad interrupts. */
void Runtime_InstallIwramAndIrqs(void)
{
    volatile u16 *ime;
    s32 zero;
    u8 *iwram;
    s32 value;
    s32 one;

    ime = &REG_IME;
    zero = 0;
    /* FAKEMATCH: the do-while keeps the REG_IME store ahead of the IWRAM
       address load. */
    do {
        *ime = zero;
    } while (0);
    iwram = IwramIrqMain;
    Dma_Set((const void *)IwramRuntime_Rom, iwram, 0x84000500, REG_DMA3);
    Data_03007ffc = iwram;
    Dma_Set((const void *)Runtime_IrqHandlers, (void *)gIrqHandlers, 0x8400000e, REG_DMA3);
    REG_DISPSTAT = zero;
    value = 0xc3ff;
    REG_KEYCNT = value;
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    /* The European editions also take the Game Pak interrupt. */
    value = 0x3001;
#else
    value = 0x1001;
#endif
    REG_IE = value;
    one = 1;
    *ime = one;
}
