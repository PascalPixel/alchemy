#include "TYPES.H"

typedef void (*InterruptHandler)(void);

extern InterruptHandler Data_030000e0[];
void Scheduler_ReservedNoOp(void);

/* Installs or removes the handler for one interrupt with IME off: sets
   the IE bit, the DISPSTAT enable (and the VCOUNT target for IRQ 2), and
   the IWRAM table entry, which falls back to the no-op handler. */
void Runtime_SetIrqHandler(u32 irq, s32 vcount, InterruptHandler handler)
{
    if (irq <= 13) {
        volatile u16 *ime_reg = (volatile u16 *)0x04000208;
        u32 ime;
        u32 bit;
        u32 ie;

        ime = *ime_reg;
        *ime_reg = (u32)ime_reg;
        /* FAKEMATCH: an empty do-while keeps the IE address load after the
           IME write */
        do {
        } while (0);
        bit = 1 << irq;
        ie = *(volatile u16 *)0x04000200 & ~bit;
        if (handler != 0)
            ie |= bit;
        *(volatile u16 *)0x04000200 = ie;
        if (irq <= 2) {
            u32 enable = 8 << irq;
            u32 keep = ~enable;
            u32 stat;

            if (irq == 2) {
                enable |= vcount << 8;
                keep &= 0xff;
            }
            stat = *(volatile u16 *)0x04000004 & keep;
            if (handler != 0)
                stat |= enable;
            *(volatile u16 *)0x04000004 = stat;
        }
        if (handler != 0)
            Data_030000e0[irq] = handler;
        else
            Data_030000e0[irq] = Scheduler_ReservedNoOp;
        *(volatile u16 *)0x04000208 = ime;
    }
}
