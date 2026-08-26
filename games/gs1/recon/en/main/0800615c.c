#include "serial_runtime_family.h"

u8 Func_0800615c(void *payload)
{
    struct SerialRuntime *state;
    u32 channel_state;
    u32 unused;
    s32 channel;

    state = SERIAL_RUNTIME;
    unused = 0;
    REG_IME = 0;
    for (channel = 0; channel <= 3; channel++) {
        u16 *swap = state->ready_buffer[channel];

        state->ready_buffer[channel] = state->pending_buffer[channel];
        state->pending_buffer[channel] = swap;
    }
    channel_state = *(u32 *)state->channel_flags;
    *(u32 *)state->channel_flags = unused;
    REG_IME = 1;
    state->current_mask = 0;

    for (channel = 0; channel <= 1; channel++) {
        u16 *packet;
        s32 index;
        s32 checksum;

        packet = state->pending_buffer[channel];
        checksum = 0;
        for (index = 0; index <= 13; index++)
            checksum += packet[index];
        if (((u8 *)&channel_state)[channel] == 1 && (s16)checksum == -1) {
            START_DMA(packet + 2, (u8 *)payload + channel * 24, 0x84000006);
            state->current_mask |= 1 << channel;
        }
        if ((s16)checksum == -1)
            packet[1] = ~packet[1];
    }
    state->received_mask |= state->current_mask;
    return state->current_mask;
}
