/* Remaining: 120/120 bytes including pool; score 60, one reordered mask
   shift; physical names resolve.
   Trials: natural zero340; word-zero temporary260; address-valued store180;
   IME boundary60. Separate read/mask statements, direct/shared IE pointer
   orders, a one-pass IE read and compound shift expression did not improve
   60 (early pointer orders scored120). Retain the simpler mask expression. */
#include "TYPES.H"
#include "IO_REG.H"
#include "IRQ.H"

/* Installs or removes the handler for one interrupt with IME off: sets
   the IE bit, the DISPSTAT enable (and the VCOUNT target for IRQ 2), and
   the IWRAM table entry, which falls back to the no-op handler. */

void Runtime_SetIrqHandler(u32 irq, s32 vcount, InterruptHandler handler)
{
    if (irq <= 13) {
        volatile u16 *ime_reg = &REG_IME;
        u32 ime;
        u32 bit;
        u32 ie;

        ime = *ime_reg;
        /* FAKEMATCH: writing the IME register address keeps its disabled
           low bit while emitting the measured register store; literal zero
           scored 340 and an ordinary word zero temporary scored 260. */
        *ime_reg = (u32)ime_reg;
        /* FAKEMATCH: the empty do-while keeps the IE address load after
           the IME store; without it three instructions reorder (180). */
        do {
        } while (0);
        bit = 1 << irq;
        ie = REG_IE & ~bit;
        if (handler != 0)
            ie |= bit;
        REG_IE = ie;
        if (irq <= 2) {
            u32 enable = 8 << irq;
            u32 keep = ~enable;
            u32 stat;

            if (irq == 2) {
                enable |= vcount << 8;
                keep &= 0xff;
            }
            stat = REG_DISPSTAT & keep;
            if (handler != 0)
                stat |= enable;
            REG_DISPSTAT = stat;
        }
        if (handler != 0)
            gIrqHandlers[irq] = handler;
        else
            gIrqHandlers[irq] = Runtime_IgnoreInterrupt;
        REG_IME = ime;
    }
}
