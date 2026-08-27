#include "serial_runtime_family.h"

/*
 * The reference emits a single Thumb `stmia r3!, {r0, r1, r2}` (with a
 * `subs r3, #12` afterward to undo the mandatory Thumb STM writeback) for
 * this DMA3 descriptor write, and no matching `ldmia` anywhere near it.
 * gs1cc's generic store-multiple combiner (arm.c store_multiple_sequence)
 * merges consecutive same-base, ascending, adjacent-offset word stores
 * into one STM, but explicitly refuses to do so when the memory operand
 * is volatile ("Don't reorder volatile memory references"). The
 * TU-wide DMA3 macro types its target as `volatile struct DmaRegisters
 * *`, which would suppress that combine and force three separate `str`
 * instructions -- not what the reference shows. A non-volatile alias
 * to the same three-word register block lets the ordinary field stores
 * below combine the way the reference does.
 */
struct DmaRegistersRW {
    u32 source;
    u32 destination;
    u32 control;
};
#define DMA3_RW ((struct DmaRegistersRW *)0x040000d4)

void Func_08005d10(void)
{
    u32 interrupt_enable;
    u32 zero;
    s32 index;
    volatile u16 *ime_reg;

    interrupt_enable = REG_IME;
    ime_reg = &REG_IME;
    *ime_reg = (u32)ime_reg;
    Func_0800307c(7, 0, Func_08006240);
    Func_0800307c(6, 0, Func_08006240);
    REG_IME = 0;

    REG_IE &= 0xff3f;
    if ((REG_IF & 0x80) != 0)
        REG_IF = 0x80;
    if ((REG_IF & 0x40) != 0)
        REG_IF = 0x40;

    REG_RCNT = 0x8000;
    REG_RCNT = 0;
    REG_SIOCNT = 0x1000;
    REG_RCNT = 0;
    REG_SIOCNT = 0x2000;
    REG_SIOCNT16 = REG_SIOCNT16 | 0x4003;

    REG_IME = 1;
    zero = 0;
    DMA3_RW->source = (u32)&zero;
    DMA3_RW->destination = (u32)SERIAL_RUNTIME;
    DMA3_RW->control = 0x85000058;
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
    REG_IME = 1;
    *(volatile u16 *)ADDR_03001CB0 = 1;
    *(u8 *)0x020023a0 = 0;
    SERIAL_ACTIVE_A = 0;
    SERIAL_VALUE_A = 0;
    SERIAL_ACTIVE_B = 0;
    SERIAL_VALUE_B = 0;
    Func_0800651c();
    REG_IME = interrupt_enable;
}
