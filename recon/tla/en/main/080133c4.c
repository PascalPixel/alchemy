#include "TYPES.H"
#include "DMA.H"
#include "IO_REG.H"
#include "IRQ.H"

/* Remaining: complete extent 112/116 bytes including the pool; fresh
   score 695: 5 register-only, 4 operand, 3 reordered,
   2 inserted and 2 deleted differences. The compiler retains IwramIrqMain
   across the first DMA in r4, saves r5/r6 rather than r5, and shares the
   bank's pool entry; vector/table loads and later stores also differ.
   Trials: direct registers 2150 with unresolved names; ordinary TBS
   locals 890; one-pass IME store 795; direct bank/void casts stayed 795;
   one-pass vector store 695. On 2026-10-03, a direct volatile BIOS-vector
   store and moving the boundary to the runtime-copy phase both scored
   795; the latter compiled to 112/116 bytes including its pool. Restore
   the stronger vector-boundary form. No flags or compiler changes. */
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
        Data_03007ffc = IwramIrqMain;
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
