#include "serial_runtime_family.h"

s32 Func_08005ee0(void *send, void *receive)
{
    volatile u32 *sio;
    u32 control;
    struct SerialRuntime *state;
    s32 packed;
    s32 result;

    state = SERIAL_RUNTIME;
    sio = (volatile u32 *)0x04000128;
    control = *sio;
    switch (state->phase) {
    case 0: {
        u8 mask30 = control & 0x30;
        u8 mode;

        if (mask30 == 0 && (mode = control & 0x88) == 8) {
            u8 serial_error = control & 4;

            if (serial_error == 0 && state->send_index == -1) {
                u32 ie;

                REG_IME = serial_error;
                ie = REG_IE;
                ie &= ~0x80;
                ie |= 0x40;
                REG_IE = ie;
                REG_IME = 1;
                *((volatile u8 *)sio + 1) &= ~0x40;
                REG_IF = 0xc0;
                REG_TM3CNT = 0xc963;
                state->mode = mode;
            }
        } else if (mask30 != 0) {
            /* fall through */
        } else {
            break;
        }
        state->phase = 1;
    }
    case 1:
        Func_0800615c(receive);
        Func_080060e8(send);
        break;
    }

    SERIAL_RUNTIME->sequence++;
    packed = SERIAL_RUNTIME->current_mask | (SERIAL_RUNTIME->received_mask << 8);
    if (SERIAL_RUNTIME->mode == 8)
        packed |= 0x80;
    result = packed;
    if (SERIAL_RUNTIME->is_parent != 0)
        result |= 0x1000;
    if (((control << 26) >> 30) > 1)
        result |= 0x2000;
    return result;
}
