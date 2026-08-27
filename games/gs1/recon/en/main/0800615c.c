#include "serial_runtime_family.h"

u8 Func_0800615c(void *payload)
{
    struct SerialRuntime *state;
    u32 channel_state[2];
    s32 channel;
    u16 **ready_ptr;
    u16 **pp;
    u8 *dst;

    channel_state[1] = 0;
    REG_IME = 0;
    state = SERIAL_RUNTIME;
    ready_ptr = state->ready_buffer;
    channel = 3;
    do {
        u16 *swap = ready_ptr[4];

        ready_ptr[4] = *ready_ptr;
        *ready_ptr++ = swap;
    } while (--channel >= 0);
    channel_state[0] = *(u32 *)SERIAL_RUNTIME->channel_flags;
    *(u32 *)SERIAL_RUNTIME->channel_flags = 0;
    REG_IME = 1;
    SERIAL_RUNTIME->current_mask = 0;

    pp = state->pending_buffer;
    dst = (u8 *)payload;
    for (channel = 0; channel <= 1; channel++) {
        u16 *packet;
        u32 index;
        s32 checksum;

        packet = *pp;
        checksum = 0;
        for (index = 0; index <= 13; index++)
            checksum += packet[index];
        if (((u8 *)&channel_state[0])[channel] == 1 && (s16)checksum == -1) {
            SERIAL_RUNTIME->current_mask |= 1 << channel;
            START_DMA(*pp + 2, dst, 0x84000006);
        }
        if ((s16)checksum == -1)
            packet[1] = ~packet[1];
        pp++;
        dst += 24;
    }
    state->received_mask |= state->current_mask;
    return state->current_mask;
}
