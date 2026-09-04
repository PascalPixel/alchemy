#include "types.h"

#ifndef SERIAL_RUNTIME_TU
struct SerialRuntime {
    u8 mode;
    u8 phase;
    u8 received_mask;
    u8 current_mask;
    u8 channel_flags[4];
    u8 transfer_enabled;
    u8 is_parent;
    u8 reserved_0a;
    u8 sequence;
    u8 reserved_0c[8];
    s32 send_index;
    s32 receive_index[2];
    u8 reserved_20[8];
    u16 *send_buffer[2];
    u16 *incoming_buffer[4];
    u16 *ready_buffer[4];
    u16 *pending_buffer[4];
    u8 storage[0x100];
};

#define SERIAL_RUNTIME ((struct SerialRuntime *)0x02002240)
#define REG_SIOCNT (*(volatile u32 *)0x04000128)

void Func_080060e8(void *payload);
u8 Func_0800615c(void *payload);
#endif

s32 Func_08006088(s32 arg0, s32 arg1)
{
    u32 control = REG_SIOCNT;
    struct SerialRuntime *state = SERIAL_RUNTIME;
    s32 packed;
    s32 result;

    if (state->phase == 1) {
        Func_0800615c((void *)arg1);
        Func_080060e8((void *)arg0);
        state->sequence++;
    }
    packed = state->current_mask;
    packed |= state->received_mask << 8;
    if (state->mode == 8)
        packed |= 0x80;
    result = packed;
    if (state->is_parent != 0)
        result |= 0x1000;
    if (((control << 26) >> 30) > 1)
        result |= 0x2000;
    return result;
}
