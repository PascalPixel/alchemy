/* Draft, not exact (2026-09-24): candidate=350 reference=350 differing_halfwords=94. Constants the reference loads from
   the literal pool are spelled as link-time Value_ symbols, which restores
   the reference size; wraps marked FAKEMATCH only move scheduling. */
#include "TYPES.H"
extern u8 Value_0000ff3f;
extern u8 Value_00008000;
extern u8 Value_00001000;
extern u8 Value_00002000;
extern u8 Value_00004003;
#include "serial_runtime_family.h"
#include "DMA.H"

/* Reset the serial runtime and install its two interrupt handlers. */
void Func_08005d10(void)
{
    u32 interrupt_enable;
    u32 zero;
    s32 index;
    volatile u16 *ime_reg;

    interrupt_enable = REG_IME;
    do { ime_reg = &REG_IME; } while (0); /* FAKEMATCH */
    *ime_reg = (u32)ime_reg;
    Func_0800307c(7, 0, Func_08006240);
    Func_0800307c(6, 0, Func_08006240);
    REG_IME = 0;

    REG_IE &= 0xff3f;
    if ((REG_IF & 0x80) != 0)
        REG_IF = 0x80;
    if ((REG_IF & 0x40) != 0)
        REG_IF = 0x40;

    REG_RCNT = (s32)&Value_00008000;
    REG_RCNT = 0;
    REG_SIOCNT = 0x1000;
    REG_RCNT = 0;
    REG_SIOCNT = 0x2000;
    REG_SIOCNT16 = REG_SIOCNT16 | 0x4003;

    REG_IME = 1;
    zero = 0;
    Dma_Set(&zero, SERIAL_RUNTIME, 0x85000058, (volatile u32 *)0x040000d4);
    SERIAL_RUNTIME->send_index = -1;
    SERIAL_RUNTIME->send_buffer[0] = (u16 *)(SERIAL_RUNTIME->storage + 0);
    SERIAL_RUNTIME->send_buffer[1] = (u16 *)(SERIAL_RUNTIME->storage + 32);
    for (index = 0; index <= 1; index++) {
        SERIAL_RUNTIME->incoming_buffer[index] = (u16 *)(SERIAL_RUNTIME->storage + 64 + index * 96);
        SERIAL_RUNTIME->ready_buffer[index] = (u16 *)(SERIAL_RUNTIME->storage + 96 + index * 96);
        SERIAL_RUNTIME->pending_buffer[index] = (u16 *)(SERIAL_RUNTIME->storage + 128 + index * 96);
    }

    REG_IME = 0;
    REG_IE |= 0x80;
    do { REG_IME = 1; } while (0); /* FAKEMATCH */
    *(volatile u16 *)ADDR_03001CB0 = 1;
    *(u8 *)0x020023a0 = 0;
    SERIAL_ACTIVE_A = 0;
    SERIAL_VALUE_A = 0;
    SERIAL_ACTIVE_B = 0;
    SERIAL_VALUE_B = 0;
    Func_0800651c();
    REG_IME = interrupt_enable;
}
