/* Draft, differ 2 of 121: at the end the native loads the halfword constant
 * 0x80 from the pool before the volatile read of SIOCNT (ldr r2, =0x80; ldrh
 * r3, [r1]), this reads SIOCNT first. Measured for that: the operands either
 * way round, a cast, a u16 or u32 temporary (differ 16, a movs or an extra
 * extension), barriers before or after the timer store, the register through
 * a pinned pointer (all differ 2 or worse); three minutes of permuter found
 * nothing. Fixed so far: sio_control set before the data read (the permuter's
 * form, differ 6 to 4) and the send index read into a temporary before a
 * barrier, so it loads before the is_parent store (differ 4 to 2). */
#include "serial_runtime.h"
#include "io_reg.h"

void SerialRuntime_HandleTransferInterrupt(void)
{
    struct SerialRuntime *send_state;
    struct SerialRuntime *receive_state;
    struct SerialRuntime *tail_state;
    u16 recv[4];
    volatile u32 *sio_control;
    s32 channel;

    sio_control = &REG_SIOCNT;
    *(u64 *)recv = *(volatile u64 *)REG_SIODATA32;
    send_state = &gSerialRuntime;
    {
        s32 index;
        u8 is_parent = (*sio_control << 25) >> 31;

        index = send_state->send_index;
        asm volatile("");
        send_state->is_parent = is_parent;
        if (index == -1) {
            u32 idle_word = 0xfefe;
            u16 *swap;

            ((volatile u16 *)sio_control)[1] = idle_word;
            swap = send_state->send_buffer[1];
            send_state->send_buffer[1] = send_state->send_buffer[0];
            send_state->send_buffer[0] = swap;
        } else if (index >= 0) {
            ((volatile u16 *)sio_control)[1] = send_state->send_buffer[1][index];
        }
    }
    receive_state = &gSerialRuntime;
    if (receive_state->send_index <= 14)
        receive_state->send_index++;

    channel = 0;
receive_loop:
    {
        s32 receive_index;
        u16 *incoming;

        if (recv[channel] == 0xfefe && receive_state->receive_index[channel] > 13) {
            receive_state->receive_index[channel] = -1;
        } else {
            receive_index = receive_state->receive_index[channel];
            incoming = receive_state->incoming_buffer[channel];
            incoming[receive_index] = recv[channel];
            if (receive_index == 13) {
                u16 *swap = receive_state->ready_buffer[channel];

                receive_state->ready_buffer[channel] = incoming;
                receive_state->incoming_buffer[channel] = swap;
                receive_state->channel_flags[channel] |= 1;
            }
        }
    }
    tail_state = &gSerialRuntime;
    if (tail_state->is_parent != 0)
        tail_state->channel_flags[channel] |= 2;
    if (tail_state->receive_index[channel] <= 14)
        tail_state->receive_index[channel]++;
    channel++;
    receive_state = tail_state;
    if (channel <= 1)
        goto receive_loop;

    if (tail_state->mode == 8) {
        REG_TM3CNT_H = 0;
        asm volatile("");
        REG_SIOCNT16 |= 0x80;
        REG_TM3CNT_H = 0xc0;
    }
}
