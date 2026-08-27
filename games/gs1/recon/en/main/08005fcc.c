#include "serial_runtime_family.h"

s32 Func_08005fcc(void)
{
    struct SerialRuntime *state;
    volatile u32 *siocnt;
    u32 control;
    s32 packed;
    s32 result;

    state = SERIAL_RUNTIME;
    siocnt = &REG_SIOCNT;
    control = *siocnt;
    if (state->phase == 0) {
        u8 masked = control & 0x88;

        if (masked == 8) {
            u8 serial_error = control & 4;

            if (serial_error == 0 && state->send_index == -1) {
                u32 ie;

                REG_IME = serial_error;
                ie = REG_IE;
                ie &= ~0x80;
                ie |= 0x40;
                REG_IE = ie;
                REG_IME = 1;
                *((volatile u8 *)siocnt + 1) &= ~0x40;
                REG_IF = 0xc0;
                REG_TM3CNT = 0xc963;
                state->mode = masked;
            }
            state->phase = 1;
        }
        state->sequence++;
    }

    packed = state->current_mask | (state->received_mask << 8);
    if (state->mode == 8)
        packed |= 0x80;
    result = packed;
    if (state->is_parent != 0)
        result |= 0x1000;
    if (((control << 26) >> 30) > 1)
        result |= 0x2000;
    return result;
}
