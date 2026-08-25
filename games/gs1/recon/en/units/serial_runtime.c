#include "types.h"
#include "global_cells.h"

/* Ordered serial/runtime translation unit: 08005d10 through 0800651c. */

#define SERIAL_RUNTIME_TU

typedef void (*InterruptHandler)(void);

struct DmaRegisters {
    u32 source;
    u32 destination;
    u32 control;
};

struct SerialPacket {
    u8 sequence;
    u8 peer_pair;
    u16 checksum;
    u8 payload[24];
    u8 padding[4];
};

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

struct SerialTransferState {
    u8 active;
    u8 status;
    u8 flags;
    u8 peer_flags;
    u8 reserved[20];
    u16 value;
};

#define SERIAL_RUNTIME ((struct SerialRuntime *)0x02002240)
#define SERIAL_TRANSFER ((struct SerialTransferState *)0x02002220)
#define DMA3 ((volatile struct DmaRegisters *)0x040000d4)
#define REG_SIODATA32 ((volatile u32 *)0x04000120)
#define REG_SIOCNT (*(volatile u32 *)0x04000128)
#define REG_SIOCNT16 (*(volatile u16 *)0x04000128)
#define REG_SIODATA8 (*(volatile u16 *)0x0400012a)
#define REG_RCNT (*(volatile u16 *)0x04000134)
#define REG_TM3CNT (*(volatile u32 *)0x0400010c)
#define REG_TM3CNT_H (*(volatile u16 *)0x0400010e)
#define REG_IE (*(volatile u16 *)0x04000200)
#define REG_IF (*(volatile u16 *)0x04000202)
#define REG_IME (*(volatile u16 *)0x04000208)

#define SERIAL_ACTIVE_A (*(volatile s32 *)0x02002080)
#define SERIAL_VALUE_A (*(volatile u16 *)0x02002008)
#define SERIAL_ACTIVE_B (*(volatile s32 *)0x020023ac)
#define SERIAL_VALUE_B (*(volatile u16 *)0x02002238)
#define SERIAL_RESULT (*(volatile u8 *)0x020023a4)

#define START_DMA(source_, destination_, control_) do { \
    DMA3->source = (u32)(source_); \
    DMA3->destination = (u32)(destination_); \
    DMA3->control = (control_); \
} while (0)

void Func_0800307c(s32 index, s32 priority, InterruptHandler handler);
s32 Func_080030f8(s32 frames);
void Func_080060e8(void *payload);
u8 Func_0800615c(void *payload);
void Func_08006240(void);
void Func_0800651c(void);
extern u8 Data_00000000[];
extern u8 Data_000000c0[];
extern struct SerialTransferState Data_02002220;
extern volatile s32 Data_02002080;
extern volatile u16 Data_02002008;
extern volatile s32 Data_020023ac;
extern volatile u16 Data_02002238;
extern volatile u16 Data_04000208;

void Func_08005d10(void)
{
    struct SerialRuntime *state;
    u16 interrupt_enable;
    u32 zero;
    s32 index;

    interrupt_enable = REG_IME;
    REG_IME = (u16)&REG_IME;
    Func_0800307c(7, 0, Func_08006240);
    Func_0800307c(6, 0, Func_08006240);

    REG_IE &= 0xff3f;
    if ((REG_IF & 0x80) != 0)
        REG_IF = 0x80;
    if ((REG_IF & 0x40) != 0)
        REG_IF = 0x40;

    REG_RCNT = 0x8000;
    REG_IME = 0;
    REG_SIOCNT = 0x1000;
    REG_IME = 0;
    REG_SIOCNT = 0x2000;
    REG_SIOCNT16 = REG_SIOCNT16 | 0x4003;
    REG_IME = (u16)&REG_IME;

    state = SERIAL_RUNTIME;
    zero = 0;
    START_DMA(&zero, state, 0x85000058);
    state->send_index = -1;
    state->send_buffer[0] = (u16 *)(state->storage + 0);
    state->send_buffer[1] = (u16 *)(state->storage + 32);
    for (index = 0; index <= 1; index++) {
        state->incoming_buffer[index] = (u16 *)(state->storage + 64 + index * 96);
        state->ready_buffer[index] = (u16 *)(state->storage + 96 + index * 96);
        state->pending_buffer[index] = (u16 *)(state->storage + 128 + index * 96);
    }

    REG_IME = 0;
    REG_IE |= 0x80;
    REG_IME = 1;
    *(volatile u16 *)ADDR_03001CB0 = 1;
    *(u8 *)0x020023a0 = 0;
    SERIAL_ACTIVE_A = 0;
    SERIAL_VALUE_A = 0;
    SERIAL_ACTIVE_B = 0;
    SERIAL_VALUE_B = 0;
    Func_0800651c();
    REG_IME = interrupt_enable;
}

#include "../../../src/link/serial/enable_transfer_timer.c"
#include "../../../src/link/serial/disable_serial_transfer.c"

s32 Func_08005ee0(void *send, void *receive)
{
    struct SerialRuntime *state;
    u32 control;
    s32 packed;
    s32 result;

    state = SERIAL_RUNTIME;
    control = REG_SIOCNT;
    switch (state->phase) {
    case 0:
        if ((control & 0x30) == 0 && (control & 0x88) == 8) {
            u8 serial_error = control & 4;

            if (serial_error == 0 && state->send_index == -1) {
                REG_IME = serial_error;
                REG_IE = (REG_IE & ~0x80) | 0x40;
                REG_IME = 1;
                *((volatile u8 *)0x04000128 + 1) &= ~0x40;
                REG_IF = 0xc0;
                REG_TM3CNT = 0xc963;
                state->mode = control & 0x88;
            }
            state->phase = 1;
        } else if ((control & 0x30) != 0) {
            state->phase = 1;
        } else {
            break;
        }
    case 1:
        Func_0800615c(receive);
        Func_080060e8(send);
        state = SERIAL_RUNTIME;
        break;
    }

    state->sequence++;
    packed = state->current_mask | (state->received_mask << 8);
    if (state->mode == 8)
        packed |= 0x80;
    result = packed;
    if (state->is_parent != 0)
        result |= 0x1000;
    if (((control << 26) >> 30) > 1)
        result |= 0x2000;
    return result;
}

s32 Func_08005fcc(void)
{
    struct SerialRuntime *state;
    u32 control;
    s32 packed;
    s32 result;

    state = SERIAL_RUNTIME;
    control = REG_SIOCNT;
    if (state->phase == 0) {
        if ((control & 0x88) == 8) {
            u8 serial_error = control & 4;

            if (serial_error == 0 && state->send_index == -1) {
                REG_IME = serial_error;
                REG_IE = (REG_IE & ~0x80) | 0x40;
                REG_IME = 1;
                *((volatile u8 *)0x04000128 + 1) &= ~0x40;
                REG_IF = 0xc0;
                REG_TM3CNT = 0xc963;
                state->mode = control & 0x88;
            }
            state->phase = 1;
        }
        state->sequence++;
    }

    packed = state->current_mask | (state->received_mask << 8);
    if (state->mode == 8)
        packed |= 0x80;
    result = packed;
    if (state->is_parent != 0)
        result |= 0x1000;
    if (((control << 26) >> 30) > 1)
        result |= 0x2000;
    return result;
}

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

void Func_080060e8(void *payload)
{
    volatile struct SerialRuntime *state;
    u32 index;
    u32 checksum;

    state = SERIAL_RUNTIME;
    {
        u8 *packet;
        u8 current_mask;
        u8 received_mask;

        packet = (u8 *)state->send_buffer[0];
        packet[0] = state->sequence;
        current_mask = state->current_mask;
        received_mask = state->received_mask;
        checksum = 0;
        packet[1] = received_mask ^ current_mask;
        *(u16 *)(packet + 2) = checksum;
        /* The reference's standalone three-register Thumb STMIA is not
         * emitted by canonical GCC 2.96 from an ordinary-C aggregate store. */
        *DMA3 = (struct DmaRegisters){
            (u32)payload,
            (u32)(packet + 4),
            0x84000006
        };
    }
    {
        u16 *packet;
        u32 value;

        packet = state->send_buffer[0];
        for (index = 0; index <= 13; index++) {
            value = *packet;
            packet++;
            checksum += value;
        }
    }
    {
        u32 inverse;
        struct SerialPacket *packet;

        inverse = ~checksum;
        packet = (struct SerialPacket *)state->send_buffer[0];
        packet->checksum = inverse;
    }
    if (state->mode != 0)
        REG_TM3CNT_H = 0;
    state->send_index = -1;
    if (state->mode != 0 && state->transfer_enabled != 0)
        REG_TM3CNT_H = (u32)Data_000000c0;
}

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

#include "../../../src/link/serial/handle_transfer_interrupt.c"
#include "../../../src/link/serial/remove_serial_irq_handlers.c"
#include "../../../src/link/serial/wait_for_status_mask.c"

s32 Func_080063bc(s32 value, s32 transfer_value)
{
    struct SerialTransferState *state;
    u32 saved_interrupt_master;
    s32 result;

    state = SERIAL_TRANSFER;
    if (SERIAL_ACTIVE_A != 0)
        return -1;
    do {
        saved_interrupt_master = Data_04000208;
        Data_04000208 = (u16)&Data_04000208;
        do {
            state->status = 0x80;
            SERIAL_VALUE_A = transfer_value;
            SERIAL_RESULT = 0;
            SERIAL_ACTIVE_A = value;
            state->active = 1;
        } while (0);
        Data_04000208 = saved_interrupt_master;
        result = 0;
    } while (0);
    return result;
}

s32 Func_08006408(s32 value)
{
    struct SerialTransferState *state;
    volatile s32 *active;
    u32 saved_interrupt_master;
    s32 current;
    s32 result;

    active = (volatile s32 *)0x020023ac;
    current = *active;
    state = SERIAL_TRANSFER;
    if (current == 0)
        goto start_transfer;
    result = -1;
    goto finish;
start_transfer:
    saved_interrupt_master = Data_04000208;
    Data_04000208 = (u16)&Data_04000208;
    do {
        state->status = 0x81;
        SERIAL_VALUE_B = current;
        state->active = 1;
        *active = value;
        SERIAL_RESULT = (u32)Data_00000000;
    } while (0);
    Data_04000208 = saved_interrupt_master;
    result = 0;
finish:
    return result;
}

#include "../../../src/link/serial/wait_for_transfer_a.c"
#include "../../../src/link/serial/wait_for_transfer_b.c"
#include "../../../src/link/serial/wait_for_transfers.c"
#include "../../../src/link/serial/get_active_transfers.c"

void Func_0800651c(void)
{
    struct SerialTransferState *state;
    volatile u16 *interrupt_master;
    u32 saved_interrupt_master;
    u32 zero;

    do {
        interrupt_master = &Data_04000208;
        state = &Data_02002220;
        zero = 0;
    } while (0);
    do {
        saved_interrupt_master = *interrupt_master;
        *interrupt_master = (u16)interrupt_master;
        do {
            state->status = 0x80;
            Data_02002080 = zero;
            Data_02002008 = zero;
            Data_020023ac = zero;
            state->peer_flags = zero;
            state->flags = zero;
            Data_02002238 = zero;
        } while (0);
        *interrupt_master = saved_interrupt_master;
    } while (0);
}
