#include "types.h"

struct LinkState_08006240 {
    u8 mode;
    u8 unknown_01[3];
    u8 channel_status[2];
    u8 unknown_06[3];
    u8 transfer_error;
    u8 unknown_0a[0x0a];
    s32 send_index;
    s32 receive_index[2];
    u8 unknown_20[8];
    u16 *pending_send_buffer;
    u16 *send_buffer;
    u16 *receive_buffer[2];
    u8 unknown_38[8];
    u16 *completed_receive_buffer[2];
};

/*
 * Service one multiplayer serial transfer.  Complete sixteen-halfword receive
 * buffers are exchanged with their consumer buffers, while FEFE synchronizes
 * the transmit and per-channel receive cursors.
 */
void Func_08006240(void)
{
    volatile struct LinkState_08006240 *state =
        (volatile struct LinkState_08006240 *)0x02002240;
    volatile u16 *serial_control = (volatile u16 *)0x04000128;
    volatile u16 *serial_send = (volatile u16 *)0x0400012a;
    u32 incoming_words[2];
    u16 *incoming = (u16 *)incoming_words;
    u32 channel;

    incoming_words[0] = *(volatile u32 *)0x04000120;
    incoming_words[1] = *(volatile u32 *)0x04000124;
    state->transfer_error = (*serial_control >> 6) & 1;

    if (state->send_index == -1) {
        u16 *buffer;

        *serial_send = 0xfefe;
        buffer = state->pending_send_buffer;
        state->pending_send_buffer = state->send_buffer;
        state->send_buffer = buffer;
    } else if (state->send_index >= 0) {
        *serial_send = state->send_buffer[state->send_index];
    }

    if (state->send_index <= 14) {
        state->send_index++;
    }

    for (channel = 0; channel < 2; channel++) {
        s32 index = state->receive_index[channel];

        if (incoming[channel] == 0xfefe && index > 13) {
            state->receive_index[channel] = -1;
        } else {
            state->receive_buffer[channel][index] = incoming[channel];
            if (index == 13) {
                u16 *buffer = state->completed_receive_buffer[channel];

                state->completed_receive_buffer[channel] =
                    state->receive_buffer[channel];
                state->receive_buffer[channel] = buffer;
                state->channel_status[channel] |= 1;
            }
        }

        if (state->transfer_error != 0) {
            state->channel_status[channel] |= 2;
        }
        if (state->receive_index[channel] <= 14) {
            state->receive_index[channel]++;
        }
    }

    if (state->mode == 8) {
        volatile u16 *timer3_control = (volatile u16 *)0x0400010e;

        *timer3_control = 0;
        *serial_control |= 0x80;
        *timer3_control = 0xc0;
    }
}
