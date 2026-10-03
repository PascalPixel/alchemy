/* Current retained draft: complete candidate/reference 280/280 bytes,
 * including the pool; score 120, two reordered instructions (the SIOCNT
 * address load and send-index read). All physical names resolve.
 * Plain local union snapshot trial: complete 276/280 bytes, score 430
 * (2 register-only, 4 operand, 4 reordered, 1 deleted). It delays stack
 * allocation and the snapshot pointer in lr, uses SP-relative stores,
 * and reverses the indexed halfword operands. Retained the closer body
 * with its measured TLA steering tagged inside the function.
 */
#include "TYPES.H"
#include "SERIAL_RUNTIME.H"


void SerialRuntime_HandleTransferInterrupt(void)
{
    struct SerialRuntime *send_state;
    struct SerialRuntime *receive_state;
    struct SerialRuntime *tail_state;
    /* FAKEMATCH: the plain TLA union snapshot scores 430 at 276 bytes;
       it delays stack allocation and the snapshot pointer in lr, uses
       SP-relative stores, and reverses indexed halfword operands.
       This volatile local/restricted view retains the closer order and
       register choices: score 120 at the complete 280-byte extent. */
    volatile union SerialDataRegisters serial_data;
    union SerialDataRegisters *const __restrict serial_snapshot =
        (union SerialDataRegisters *)&serial_data;
    volatile u32 *sio_control;
    s32 channel;

    sio_control = (volatile u32 *)0x04000128;
    /* Capture the volatile hardware once for this interrupt. */
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
