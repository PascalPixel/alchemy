#include "TYPES.H"
#include "SERIAL_RUNTIME.H"
extern u8 Data_03001cb0[];

void SerialRuntime_HandleTransferInterrupt(void)
{
    struct SerialRuntime *send_state;
    struct SerialRuntime *receive_state;
    struct SerialRuntime *tail_state;
    volatile union SerialDataRegisters serial_data;
    union SerialDataRegisters *const __restrict serial_snapshot =
        (union SerialDataRegisters *)&serial_data;
    volatile u32 *sio_control;
    s32 channel;

    sio_control = (volatile u32 *)0x04000128;
    /* Capture volatile I/O through one stable, restricted local view. */
    *serial_snapshot =
        *(volatile union SerialDataRegisters *)REG_SIODATA32;
    send_state = &gSerialRuntime;
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
    receive_state = &gSerialRuntime;
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
    tail_state = &gSerialRuntime;
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

void Runtime_SetIrqHandler(s32, s32, InterruptHandler);

void SerialRuntime_RemoveIrqHandlers(void)
{
    s16 *work;
    s32 handler;

    work = (s16 *)((u32)&Data_03001cb0);
    do {
        do {
        } while (0);
        *work = 0;
        Runtime_SetIrqHandler(7, 0, (InterruptHandler)(handler = 0));
    } while (0);
    handler = 6;
    Runtime_SetIrqHandler(handler, 0, 0);
}

extern volatile u16 gLinkStatus;
extern volatile u32 Data_04000128;
s32 WaitFrames(s32);

u32 SerialRuntime_WaitForStatusMask(s32 mask)
{
    if ((mask & gLinkStatus) != mask) {
        do {
            WaitFrames(1);
        } while ((mask & gLinkStatus) != mask);
    }
    return (Data_04000128 << 0x1A) >> 0x1E;
}
