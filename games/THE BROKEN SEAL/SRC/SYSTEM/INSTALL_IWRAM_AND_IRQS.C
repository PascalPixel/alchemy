#include "TYPES.H"
#include "DMA.H"

/* IWRAM start, where the runtime (SYSTEM/RUNTIME.S) is copied and the
   interrupt vector points. */
extern u8 Data_03000000[];
extern void *Data_03007ffc;

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

    ime = (volatile u16 *)0x04000208;
    zero = 0;
    /* FAKEMATCH: the do-while keeps the REG_IME store ahead of the IWRAM
       address load. */
    do {
        *ime = zero;
    } while (0);
    iwram = Data_03000000;
    Dma_Set((const void *)0x08000770, iwram, 0x84000500, (volatile u32 *)0x040000d4);
    Data_03007ffc = iwram;
    Dma_Set((const void *)0x08007320, (void *)0x030000e0, 0x8400000e, (volatile u32 *)0x040000d4);
    *(volatile u16 *)0x04000004 = zero;
    value = 0xc3ff;
    *(volatile u16 *)0x04000132 = value;
    value = 0x1001;
    *(volatile u16 *)0x04000200 = value;
    one = 1;
    *ime = one;
}
