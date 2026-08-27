#include "serial_runtime_family.h"

u8 Func_0800615c(void *payload)
{
    struct SerialRuntime *state;
    u32 channel_state;
    s32 channel;
    u16 **ready_ptr;

    REG_IME = 0;
    state = SERIAL_RUNTIME;
    ready_ptr = state->ready_buffer;
    channel = 3;
    do {
        u16 *swap = ready_ptr[4];

        ready_ptr[4] = *ready_ptr;
        *ready_ptr++ = swap;
    } while (--channel >= 0);
    channel_state = *(u32 *)SERIAL_RUNTIME->channel_flags;
    *(u32 *)SERIAL_RUNTIME->channel_flags = 0;
    REG_IME = 1;
    SERIAL_RUNTIME->current_mask = 0;

    for (channel = 0; channel <= 1; channel++) {
        u16 *packet;
        u32 index;
        s32 checksum;

        packet = state->pending_buffer[channel];
        checksum = 0;
        for (index = 0; index <= 13; index++)
            checksum += packet[index];
        if (((u8 *)&channel_state)[channel] == 1 && (s16)checksum == -1) {
            SERIAL_RUNTIME->current_mask |= 1 << channel;
            START_DMA(state->pending_buffer[channel] + 2, (u8 *)payload + channel * 24, 0x84000006);
        }
        if ((s16)checksum == -1)
            packet[1] = ~packet[1];
    }
    state->received_mask |= state->current_mask;
    return state->current_mask;
}
