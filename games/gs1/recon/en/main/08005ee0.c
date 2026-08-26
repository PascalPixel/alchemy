#include "serial_runtime_family.h"

s32 Func_08005ee0(void *send, void *receive)
{
    struct SerialRuntime *state;
    u32 control;
    s32 packed;
    s32 result;

    state = SERIAL_RUNTIME;
    control = REG_SIOCNT;
    switch (state->phase) {
    case 0:
        if ((control & 0x30) == 0 && (control & 0x88) == 8) {
            u8 serial_error = control & 4;

            if (serial_error == 0 && state->send_index == -1) {
                REG_IME = serial_error;
                REG_IE = (REG_IE & ~0x80) | 0x40;
                REG_IME = 1;
                *((volatile u8 *)0x04000128 + 1) &= ~0x40;
                REG_IF = 0xc0;
                REG_TM3CNT = 0xc963;
                state->mode = control & 0x88;
            }
            state->phase = 1;
        } else if ((control & 0x30) != 0) {
            state->phase = 1;
        } else {
            break;
        }
    case 1:
        Func_0800615c(receive);
        Func_080060e8(send);
        state = SERIAL_RUNTIME;
        break;
    }

    state->sequence++;
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
