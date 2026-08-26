#include "serial_runtime_family.h"

void Func_08005d10(void)
{
    struct SerialRuntime *state;
    u16 interrupt_enable;
    u32 zero;
    s32 index;

    interrupt_enable = REG_IME;
    REG_IME = (u16)&REG_IME;
    Func_0800307c(7, 0, Func_08006240);
    Func_0800307c(6, 0, Func_08006240);

    REG_IE &= 0xff3f;
    if ((REG_IF & 0x80) != 0)
        REG_IF = 0x80;
    if ((REG_IF & 0x40) != 0)
        REG_IF = 0x40;

    REG_RCNT = 0x8000;
    REG_IME = 0;
    REG_SIOCNT = 0x1000;
    REG_IME = 0;
    REG_SIOCNT = 0x2000;
    REG_SIOCNT16 = REG_SIOCNT16 | 0x4003;
    REG_IME = (u16)&REG_IME;

    state = SERIAL_RUNTIME;
    zero = 0;
    START_DMA(&zero, state, 0x85000058);
    state->send_index = -1;
    state->send_buffer[0] = (u16 *)(state->storage + 0);
    state->send_buffer[1] = (u16 *)(state->storage + 32);
    for (index = 0; index <= 1; index++) {
        state->incoming_buffer[index] = (u16 *)(state->storage + 64 + index * 96);
        state->ready_buffer[index] = (u16 *)(state->storage + 96 + index * 96);
        state->pending_buffer[index] = (u16 *)(state->storage + 128 + index * 96);
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
