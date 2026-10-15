#ifndef GUARD_SERIAL_RUNTIME_H
#define GUARD_SERIAL_RUNTIME_H

#include "edition.h"
#include "types.h"
#include "layout_guard.h"
#include "system.h"
#include "io_reg.h"
#include "irq.h"
#include "dma.h"

struct SerialPacket
{
    u8  sequence;
    u8  missingMask;
    u16 checksum;
    u8  payload[24];
    u8  reservedTail[4];
};

LAYOUT_SIZE_GUARD(SerialPacket_Size, struct SerialPacket, 32);
LAYOUT_OFFSET_GUARD(SerialPacket_Sequence, struct SerialPacket, sequence, 0);
LAYOUT_OFFSET_GUARD(SerialPacket_MissingMask, struct SerialPacket, missingMask, 1);
LAYOUT_OFFSET_GUARD(SerialPacket_Checksum, struct SerialPacket, checksum, 2);
LAYOUT_OFFSET_GUARD(SerialPacket_Payload, struct SerialPacket, payload, 4);
LAYOUT_OFFSET_GUARD(SerialPacket_ReservedTail, struct SerialPacket, reservedTail, 28);

union SerialDataRegisters
{
    u32 words[2];
    u16 halfwords[4];
};

struct SerialRuntime
{
    u8   mode;
    u8   phase;
    u8   receivedMask;
    u8   currentMask;
    u8   channelFlags[4];
    u8   transferEnabled;
    u8   isParent;
    u8   reserved0a;
    u8   sequence;
    u8   reserved0c[8];
    s32  sendIndex;
    s32  receiveIndex[2];
    u8   reserved20[8];
    u16 *sendBuffer[2];
    u16 *incomingBuffer[4];
    u16 *readyBuffer[4];
    u16 *pendingBuffer[4];
    u8   storage[0x100];
};

LAYOUT_SIZE_GUARD(SerialRuntime_Size, struct SerialRuntime, 0x160);
LAYOUT_OFFSET_GUARD(SerialRuntime_Mode, struct SerialRuntime, mode, 0);
LAYOUT_OFFSET_GUARD(SerialRuntime_TransferEnabled, struct SerialRuntime, transferEnabled, 8);
LAYOUT_OFFSET_GUARD(SerialRuntime_SendIndex, struct SerialRuntime, sendIndex, 0x14);
LAYOUT_OFFSET_GUARD(SerialRuntime_Storage, struct SerialRuntime, storage, 0x60);

struct SerialTransferState
{
    u8 active;
    u8 status;
    u8 flags;
    u8 peerFlags;
    u8 payload[20];
};

LAYOUT_SIZE_GUARD(SerialTransferState_Size, struct SerialTransferState, 24);
LAYOUT_OFFSET_GUARD(SerialTransferState_Active, struct SerialTransferState, active, 0);
LAYOUT_OFFSET_GUARD(SerialTransferState_Status, struct SerialTransferState, status, 1);
LAYOUT_OFFSET_GUARD(SerialTransferState_Flags, struct SerialTransferState, flags, 2);
LAYOUT_OFFSET_GUARD(SerialTransferState_PeerFlags, struct SerialTransferState, peerFlags, 3);
LAYOUT_OFFSET_GUARD(SerialTransferState_Payload, struct SerialTransferState, payload, 4);

#define SERIAL_RUNTIME (&gSerialRuntime)
#define DMA3 ((volatile struct DmaRegisters *)0x040000d4)

#define START_DMA(source_, destination_, control_) \
    do                                             \
    {                                              \
        DMA3->source = (u32)(source_);             \
        DMA3->destination = (u32)(destination_);   \
        DMA3->control = (control_);                \
    } while (0)

#if GAME_TLA
s32  SerialRuntime_PollStatus(void);
s32  SerialRuntime_ExchangePayloads(void *send, void *receive);
void SerialRuntime_PrepareSendPacket(void *payload);
u8   SerialRuntime_CollectReceivedPayloads(void *payload);
void SerialRuntime_StepBlockTransfer(void);
void SerialRuntime_EnableTransferTimer(void);
void SerialRuntime_DisableTransfer(void);
s32  SerialRuntime_BeginTransferA(void *source, s32 size);
s32  SerialRuntime_BeginTransferB(void *destination);
void SerialRuntime_WaitForTransferA(void);
void SerialRuntime_WaitForTransferB(void);
void SerialRuntime_WaitForTransfers(void);
s32  SerialRuntime_GetActiveTransfers(void);
void BattleLink_ResetTransferState(void);
void SerialRuntime_HandleTransferInterrupt(void);
void SerialRuntime_RemoveIrqHandlers(void);
u32  SerialRuntime_WaitForStatusMask(s32 mask);
#endif
extern struct SerialTransferState gSerialTransfer;
extern struct SerialRuntime       gSerialRuntime;
extern u16                        gSerialExchangeActive;
extern s32                        gSerialSendSource;
// volatile: tagged where it steers, in SerialRuntime_StepBlockTransfer (tbs runtime.c)
extern volatile u16 gSerialSendSize;
extern s32          gSerialReceiveDest;
// volatile: tagged where it steers, in BattleLink_ReceiveActions (tbs battle.c) and LinkLobby_ReceivePartyRecords (tbs lobby.c)
extern volatile u16 gSerialReceivedSize;
extern u8           gSerialPeerPayloads[4][24];
// volatile: tagged where it steers, in SerialRuntime_StepBlockTransfer (tbs runtime.c)
extern volatile u8 gSerialBlockSequence;
extern u8          gLinkExchangeState;
// 通信の状態。VBlank の通信処理が書き換える
/* FAKEMATCH: volatile makes every access to this cell a load or store of its own; without it, in tbs-en, SaveMenu_Run: differ 221, 923 instructions for 920, and SerialRuntime_WaitForStatusMask: differ 18, 21 for 23. */
extern volatile u16 gLinkStatus;

static inline s32 SerialRuntime_AddParentFlag(s32 status)
{
    if (gSerialRuntime.isParent != 0)
        status |= 0x1000;
    return status;
}

static __inline__ void SerialRuntime_ResetTransferState(void)
{
    struct SerialTransferState *state;
    volatile u16               *ime;
    u32                         saved;
    u32                         zero;

    /* FAKEMATCH: the blocks preserve IME write order; the repeated state assignment keeps its allocation priority above the IME pointer. */
    do
    {
        state = &gSerialTransfer;
        ime = &REG_IME;
    } while (0);
    do
    {
        saved = *ime;
        *ime = (u16)ime;
    } while (0);
    do
    {
        zero = 0;
        state = &gSerialTransfer;
    } while (0);
    state->status = 0x80;
    gSerialSendSource = zero;
    gSerialSendSize = zero;
    gSerialReceiveDest = zero;
    *(u8 *)&state->peerFlags = zero;
    *(u8 *)&state->flags = zero;
    gSerialReceivedSize = zero;
    *ime = saved;
}

#endif
