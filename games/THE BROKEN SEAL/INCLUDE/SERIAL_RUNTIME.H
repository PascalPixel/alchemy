#ifndef GUARD_SERIAL_RUNTIME_H
#define GUARD_SERIAL_RUNTIME_H

#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "IO_REG.H"

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

#define SERIAL_RUNTIME (&gSerialRuntime)
#define DMA3 ((volatile struct DmaRegisters *)0x040000d4)

#define SERIAL_ACTIVE_A gSerialSendSource
#define SERIAL_VALUE_A gSerialSendSize
#define SERIAL_ACTIVE_B gSerialReceiveDest
#define SERIAL_VALUE_B gSerialReceivedSize

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
extern struct SerialTransferState gSerialTransfer;
extern struct SerialRuntime gSerialRuntime;
extern volatile s32 gSerialSendSource;
extern volatile u16 gSerialSendSize;
extern volatile s32 gSerialReceiveDest;
extern volatile u16 gSerialReceivedSize;
/* The four peers' 24-byte payloads that each frame's exchange receives;
   gSerialTransfer holds the payload this console sends. */
extern u8 gSerialPeerPayloads[4][24];
/* The block transfer's seven-bit packet sequence number. */
extern volatile u8 gSerialBlockSequence;
/* A byte the link lobby's party-record exchange writes and serial start-up
   clears, just past the runtime record. */
extern u8 gLinkExchangeState;

static inline s32 SerialRuntime_AddParentFlag(s32 status)
{
    if (gSerialRuntime.is_parent != 0)
        status |= 0x1000;
    return status;
}

static __inline__ void SerialRuntime_ResetTransferState(void)
{
    struct SerialTransferState *state;
    volatile u16 *ime;
    u32 saved;
    u32 zero;

    /* FAKEMATCH: the blocks retain the IME write order; the second state
     * assignment keeps its allocation priority above the IME pointer. */
    do {
        state = &gSerialTransfer;
        ime = &REG_IME;
    } while (0);
    do {
        saved = *ime;
        *ime = (u16)ime;
    } while (0);
    do {
        zero = 0;
        state = &gSerialTransfer;
    } while (0);
    state->status = 0x80;
    gSerialSendSource = zero;
    gSerialSendSize = zero;
    gSerialReceiveDest = zero;
    state->peer_flags = zero;
    state->flags = zero;
    gSerialReceivedSize = zero;
    *ime = saved;
}

#endif
