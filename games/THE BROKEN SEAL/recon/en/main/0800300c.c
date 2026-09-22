#include "TYPES.H"
#include "DMA.H"

/*
 * Startup interrupt/DMA init routine. Ordering and register-window
 * evidence:
 *   - REG_IME/REG_IE/REG_DISPSTAT/REG_KEYCNT addresses and values come
 *     straight from the literal pool in games/THE BROKEN SEAL/raw/0800300c.s.
 *   - The first DMA (source 0x08000770 -> dest 0x03000000, control
 *     0x84000500 = enabled|32-bit|0x500 words = 5120 bytes) matches the
 *     "relocated_iwram_payload" group's 5120-byte ROM->IWRAM copy
 *     recorded in games/THE BROKEN SEAL/raw/classification.json.
 *   - The second DMA (source 0x08007320 -> dest 0x030000e0, control
 *     0x8400000e = enabled|32-bit|0x0e words = 56 bytes) matches the
 *     "iwram_dispatch_pointers_08007320" table copy recorded in
 *     games/THE BROKEN SEAL/raw/executable_gaps/index.json (14 handler pointers,
 *     defaulting to Func_08003008/reserved_no_op_f, landing at
 *     run address 0x030000e0).
 *   - REG_IE = 0x1001 enables VBlank (bit0) + Keypad (bit12) IRQs, and
 *     REG_KEYCNT = 0xc3ff arms the keypad IRQ condition.
 *
 * Both transfers use the shared DMA machine interface in DMA.H. The
 * surrounding startup register allocation remains under reconstruction.
 */

#define REG_IME (*(volatile u16 *)0x04000208)
#define REG_IE (*(volatile u16 *)0x04000200)
#define REG_DISPSTAT (*(volatile u16 *)0x04000004)
#define REG_KEYCNT (*(volatile u16 *)0x04000132)

extern void *Data_03007ffc;

void Func_0800300c(void)
{
    REG_IME = 0;

    Dma_Set((const void *)0x08000770, (void *)0x03000000, 0x84000500,
            (volatile u32 *)0x040000d4);

    Data_03007ffc = (void *)0x03000000;

    Dma_Set((const void *)0x08007320, (void *)0x030000e0, 0x8400000e,
            (volatile u32 *)0x040000d4);

    REG_DISPSTAT = 0;
    REG_KEYCNT = 0xc3ff;
    REG_IE = 0x1001;
    REG_IME = 1;
}
