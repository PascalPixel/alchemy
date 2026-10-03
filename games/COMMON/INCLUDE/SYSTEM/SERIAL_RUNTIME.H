#ifndef GUARD_SERIAL_RUNTIME_H
#define GUARD_SERIAL_RUNTIME_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"
#include "SYSTEM.H"
#include "IO_REG.H"
#include "IRQ.H"

struct DmaRegisters {
    u32 source;
    u32 destination;
    u32 control;
};

/* The checksum covers the first 28 bytes of this 32-byte buffer slot.
   The interrupt cursors can also access the final four bytes. */
struct SerialPacket {
    u8 sequence;
    /* Previously received channels absent from this collection. */
    u8 missing_mask;
    u16 checksum;
    u8 payload[24];
    u8 reserved_tail[4];
};

LAYOUT_SIZE_GUARD(SerialPacket_Size, struct SerialPacket, 32);
LAYOUT_OFFSET_GUARD(SerialPacket_Sequence, struct SerialPacket, sequence, 0);
LAYOUT_OFFSET_GUARD(SerialPacket_MissingMask, struct SerialPacket, missing_mask, 1);
LAYOUT_OFFSET_GUARD(SerialPacket_Checksum, struct SerialPacket, checksum, 2);
LAYOUT_OFFSET_GUARD(SerialPacket_Payload, struct SerialPacket, payload, 4);
LAYOUT_OFFSET_GUARD(SerialPacket_ReservedTail, struct SerialPacket, reserved_tail, 28);

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

LAYOUT_SIZE_GUARD(SerialRuntime_Size, struct SerialRuntime, 0x160);
LAYOUT_OFFSET_GUARD(SerialRuntime_Mode, struct SerialRuntime, mode, 0);
LAYOUT_OFFSET_GUARD(SerialRuntime_TransferEnabled, struct SerialRuntime, transfer_enabled, 8);
LAYOUT_OFFSET_GUARD(SerialRuntime_SendIndex, struct SerialRuntime, send_index, 0x14);
LAYOUT_OFFSET_GUARD(SerialRuntime_Storage, struct SerialRuntime, storage, 0x60);

struct SerialTransferState {
    u8 active;
    u8 status;
    u8 flags;
    u8 peer_flags;
    /* The block transfer's 20-byte body follows its four control bytes. */
    u8 reserved[20];
};

LAYOUT_SIZE_GUARD(SerialTransferState_Size, struct SerialTransferState, 24);
LAYOUT_OFFSET_GUARD(SerialTransferState_Active, struct SerialTransferState, active, 0);
LAYOUT_OFFSET_GUARD(SerialTransferState_Status, struct SerialTransferState, status, 1);
LAYOUT_OFFSET_GUARD(SerialTransferState_Flags, struct SerialTransferState, flags, 2);
LAYOUT_OFFSET_GUARD(SerialTransferState_PeerFlags, struct SerialTransferState, peer_flags, 3);
LAYOUT_OFFSET_GUARD(SerialTransferState_Body, struct SerialTransferState, reserved, 4);

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

s32 SerialRuntime_PollStatus(void);
s32 SerialRuntime_ExchangePayloads(void *send, void *receive);
void SerialRuntime_PrepareSendPacket(void *payload);
u8 SerialRuntime_CollectReceivedPayloads(void *payload);
void SerialRuntime_StepBlockTransfer(void);
s32 SerialRuntime_BeginTransferA(void *source, s32 size);
s32 SerialRuntime_BeginTransferB(void *destination);
void SerialRuntime_WaitForTransferB(void);
void BattleLink_ResetTransferState(void);
void SerialRuntime_HandleTransferInterrupt(void);
u32 SerialRuntime_WaitForStatusMask(s32 mask);
extern struct SerialTransferState gSerialTransfer;
extern struct SerialRuntime gSerialRuntime;
/* Enables the per-VBlank payload exchange. */
extern u16 gSerialExchangeActive;
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
    /* FAKEMATCH: after trimming the false trailer, order this byte pair
       before the received-size store. Swapping ordinary statements still
       reverses flags/size; a volatile flags lvalue alone reverses the pair. */
    *(volatile u8 *)&state->peer_flags = zero;
    *(volatile u8 *)&state->flags = zero;
    gSerialReceivedSize = zero;
    *ime = saved;
}

#endif
