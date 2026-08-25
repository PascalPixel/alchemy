#include "types.h"

#ifndef SERIAL_RUNTIME_TU
union SerialDataRegisters {
    u32 words[2];
    u16 halfwords[4];
};

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
#define REG_SIODATA32 ((volatile u32 *)0x04000120)
#define REG_SIOCNT16 (*(volatile u16 *)0x04000128)
#define REG_TM3CNT_H (*(volatile u16 *)0x0400010e)
#endif

#define SerialRuntime_HandleTransferInterrupt Func_08006240

void SerialRuntime_HandleTransferInterrupt(void)
{
    struct SerialRuntime *send_state;
    struct SerialRuntime *receive_state;
    struct SerialRuntime *tail_state;
    volatile union SerialDataRegisters serial_data;
    union SerialDataRegisters * const __restrict serial_snapshot =
        (union SerialDataRegisters *)&serial_data;
    volatile u32 *sio_control;
    s32 channel;

    sio_control = (volatile u32 *)0x04000128;
    /* Capture volatile I/O through one stable, restricted local view. */
    *serial_snapshot =
        *(volatile union SerialDataRegisters *)REG_SIODATA32;
    send_state = SERIAL_RUNTIME;
    send_state->is_parent = (*sio_control << 25) >> 31;

    if (send_state->send_index == -1) {
        u32 idle_word = 0xfefe;
        u16 *swap;

        ((volatile u16 *)sio_control)[1] = idle_word;
        swap = send_state->send_buffer[1];
        send_state->send_buffer[1] = send_state->send_buffer[0];
        send_state->send_buffer[0] = swap;
    } else if (send_state->send_index >= 0) {
        ((volatile u16 *)sio_control)[1] =
            send_state->send_buffer[1][send_state->send_index];
    }
    receive_state = SERIAL_RUNTIME;
    if (receive_state->send_index <= 14)
        receive_state->send_index++;

    channel = 0;
receive_loop:
    {
        s32 receive_index;
        u16 *incoming;

        if (serial_snapshot->halfwords[channel] == 0xfefe &&
            receive_state->receive_index[channel] > 13) {
            receive_state->receive_index[channel] = -1;
        } else {
            receive_index = receive_state->receive_index[channel];
            incoming = receive_state->incoming_buffer[channel];
            incoming[receive_index] =
                serial_snapshot->halfwords[channel];
            if (receive_index == 13) {
                u16 *swap = receive_state->ready_buffer[channel];

                receive_state->ready_buffer[channel] = incoming;
                receive_state->incoming_buffer[channel] = swap;
                receive_state->channel_flags[channel] |= 1;
            }
        }
    }
    tail_state = SERIAL_RUNTIME;
    if (tail_state->is_parent != 0)
        tail_state->channel_flags[channel] |= 2;
    if (tail_state->receive_index[channel] <= 14)
        tail_state->receive_index[channel]++;
    channel++;
    receive_state = tail_state;
    if (channel <= 1) goto receive_loop;

    if (tail_state->mode == 8) {
        REG_TM3CNT_H = 0;
        REG_SIOCNT16 = REG_SIOCNT16 | 0x80;
        REG_TM3CNT_H = 0xc0;
    }
}
