#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* link/serial/enable_transfer_timer.c */
/* link/serial/enable_transfer_timer.c */
void SerialRuntime_EnableTransferTimer(void)
{
    s32 state = 0x02002240;
    if (FIELD_AT_OFFSET((void *)state, u8 *, 0) != 0)
        FIELD_AT_OFFSET((void *)state, s8 *, 8) = 1;
}

/* link/serial/disable_serial_transfer.c */
void SerialRuntime_DisableTransfer(void)
{
    s32 base;

    *(volatile u16 *)0x04000208 = 0;
    *(u16 *)0x04000200 = *(u16 *)0x04000200 & 0xff3f;
    *(volatile u16 *)0x04000208 = 1;
    *(u16 *)0x04000128 = 0x2003;
    *(u32 *)0x0400010c = 0x0000c963;
    *(u16 *)0x04000202 = 0xc0;
    base = 0x02002240;
    *((u8 *)base + 8) = 0;
}

/* link/serial/handle_transfer_interrupt.c */
/* link/serial/handle_transfer_interrupt.c */
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

/* link/serial/remove_serial_irq_handlers.c */
#ifndef SERIAL_RUNTIME_TU
typedef void (*InterruptHandler)(void);

#endif

void SerialRuntime_RemoveIrqHandlers(void)
{
    s16 *work;
    s32 handler;

    work = (s16 *)ADDR_03001CB0;
    do {
        do {
        } while (0);
        *work = 0;
        Sys_Place(7, 0, (InterruptHandler)(handler = 0));
    } while (0);
    handler = 6;
    Sys_Place(handler, 0, 0);
}

/* link/serial/wait_for_status_mask.c */
extern volatile u16 gIw;
extern volatile u32 gUnk;
s32 WaitFrames(s32);

u32 SerialRuntime_WaitForStatusMask(s32 mask)
{
    if ((mask & gIw) != mask) {
        do {
            WaitFrames(1);
        } while ((mask & gIw) != mask);
    }
    return (gUnk << 0x1A) >> 0x1E;
}

/* link/serial/wait_for_transfer_a.c */
/* link/serial/wait_for_transfer_a.c */
s32 WaitFrames(s32);

void SerialRuntime_WaitForTransferA(void)
{
    u32 work;
    u32 count;

    count = 0;
    if (*(volatile s32 *)0x02002080 != 0) {
        work = 0x02002080;
loop:
        WaitFrames(1);
        count += 1;
        if (count <= 0x927BFU) {
            if (*(volatile s32 *)work != 0) {
                goto loop;
            }
        }
    }
}

/* link/serial/wait_for_transfer_b.c */
s32 WaitFrames(s32);

void SerialRuntime_WaitForTransferB(void)
{
    s32 work;
    u32 count;
    s32 idle;

    count = 0;
    if (*(volatile s32 *)0x020023AC != 0) {
        work = 0x020023AC;
loop:
        WaitFrames(1);
        count += 1;
        idle = 0;
        if (count <= 0x927BFU) {
            if (*(volatile s32 *)work != idle) {
                goto loop;
            }
        }
    }
}

/* link/serial/wait_for_transfers.c */
s32 WaitFrames(s32);

void SerialRuntime_WaitForTransfers(void)
{
    u32 count;

    count = 0;
    if (*(volatile s32 *)0x02002080 != 0)
    {
        goto loop;
    }
    if (*(volatile s32 *)0x020023AC != 0)
    {
        goto loop;
    }
    return;
loop:
    WaitFrames(1);
    count++;
    if (count > 0x000927BF)
    {
        return;
    }
    if (*(volatile s32 *)0x02002080 != 0)
    {
        goto loop;
    }
    if (*(volatile s32 *)0x020023AC != 0)
    {
        goto loop;
    }
}

/* link/serial/get_active_transfers.c */
s32 SerialRuntime_GetActiveTransfers(void)
{
    s32 flags;

    flags = 0;
    if (*(volatile s32 *)0x02002080 != 0) {
        flags = 1;
    }
    if (*(volatile s32 *)0x020023AC != 0) {
        flags |= 2;
    }
    return flags;
}

/* runtime/wait/busy_loop_tick.c */
/* Deliberate call overhead used by the timed busy loop. */
void RuntimeWait_BusyLoopTick(void)
{
}
