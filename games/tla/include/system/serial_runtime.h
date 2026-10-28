#ifndef GUARD_SERIAL_RUNTIME_H
#define GUARD_SERIAL_RUNTIME_H

#include "edition.h"
#include "types.h"
#include "static_assert.h"
#include "system.h"
#include "io_reg.h"
#include "irq.h"
#include "dma.h"

/// A link cable packet: its packet number, the consoles not yet heard from,
/// its checksum and a 24-byte payload.
struct SerialPacket {
  u8 packetNumber; ///< SerialWork's packet number when the packet was built
  /// The consoles heard from before but not this frame (receivedMask ^
  /// currentMask).
  u8 missingMask;
  /// The complement of the sum of the packet's first 14 halfwords, taken with
  /// this field 0.
  u16 checksum;
  u8 payload[24];
  u8 reservedTail[4]; ///< past the 14 halfwords sent
};

STATIC_ASSERT(sizeof(struct SerialPacket) == 32);
STATIC_ASSERT(offsetof(struct SerialPacket, packetNumber) == 0);
STATIC_ASSERT(offsetof(struct SerialPacket, missingMask) == 1);
STATIC_ASSERT(offsetof(struct SerialPacket, checksum) == 2);
STATIC_ASSERT(offsetof(struct SerialPacket, payload) == 4);
STATIC_ASSERT(offsetof(struct SerialPacket, reservedTail) == 28);

/// The serial data registers read as two words or four halfwords; no TLA code
/// names this union.
union SerialDataRegisters {
  u32 words[2];
  u16 halfwords[4];
};

/// The link cable's runtime: its mode and phase, the consoles heard from, the
/// send position and the packet buffers the interrupt handler fills.
struct SerialWork {
  /// SIOCNT's bits 7 and 3 when this console armed timer 3 to clock the
  /// transfers: 8 then, else 0; 8 adds 0x80 to the link status.
  u8 mode;
  u8 phase;        ///< 0 looking for the partner, 1 connected
  u8 receivedMask; ///< the consoles heard from since the link began
  u8 currentMask;  ///< the consoles heard from this frame
  /// Set by the interrupt for each console and cleared each frame.
  /// bits: 1 heard while transferError was set | 0 a packet completed
  u8 channelFlags[4];
  /// Nonzero lets a new packet restart timer 3.
  u8 transferEnabled;
  /// SIOCNT's bit 6, copied by each transfer interrupt; nonzero adds 0x1000
  /// to the link status.
  u8 transferError;
  u8 reserved0a; ///< not named by TLA code
  /// The packet number, counted up each frame once connected.
  u8 packetNumber;
  u8 reserved0c[8]; ///< not named by TLA code
  /// The halfword of sendBuffer[1] sent next; -1 between packets.
  s32 sendIndex;
  /// Each console's next halfword in its incomingBuffer; -1 after an idle
  /// 0xfefe.
  s32 receiveIndex[2];
  u8 reserved20[8]; ///< not named by TLA code
  /// The packet being built ([0]) and the packet being sent ([1]).
  u16 *sendBuffer[2];
  /// Each console's packet being received; two are used.
  u16 *incomingBuffer[4];
  /// Each console's last complete packet, swapped in by the interrupt.
  u16 *readyBuffer[4];
  /// The packets the frame code checks, swapped with readyBuffer each frame.
  u16 *pendingBuffer[4];
  /// The buffers' memory: two 32-byte send buffers, then 96 bytes a console
  /// for its incoming, ready and pending packets.
  u8 storage[0x100];
};

STATIC_ASSERT(sizeof(struct SerialWork) == 0x160);
STATIC_ASSERT(offsetof(struct SerialWork, mode) == 0);
STATIC_ASSERT(offsetof(struct SerialWork, transferEnabled) == 8);
STATIC_ASSERT(offsetof(struct SerialWork, sendIndex) == 0x14);
STATIC_ASSERT(offsetof(struct SerialWork, storage) == 0x60);

/// A console's block transfer state, exchanged with the partner each VBlank:
/// its block number, its status and flags, and a 20-byte block.
struct SerialTransferState {
  u8 active; ///< 1 when a send or receive starts
  u8 status; ///< 0x80 when a send starts, 0x81 when a receive starts
  u8 flags;
  u8 peerFlags;
  u8 payload[20]; ///< one block of the data sent
};

STATIC_ASSERT(sizeof(struct SerialTransferState) == 24);
STATIC_ASSERT(offsetof(struct SerialTransferState, active) == 0);
STATIC_ASSERT(offsetof(struct SerialTransferState, status) == 1);
STATIC_ASSERT(offsetof(struct SerialTransferState, flags) == 2);
STATIC_ASSERT(offsetof(struct SerialTransferState, peerFlags) == 3);
STATIC_ASSERT(offsetof(struct SerialTransferState, payload) == 4);

#define SERIAL_RUNTIME (&gSerialWork)
#define DMA3 ((volatile struct DmaRegisters *)&REG_DMA3SAD)

#define START_DMA(source_, destination_, control_)                             \
  do {                                                                         \
    DMA3->source = (u32)(source_);                                             \
    DMA3->destination = (u32)(destination_);                                   \
    DMA3->control = (control_);                                                \
  } while (0)

s32 Serial_PollStatus(void);
s32 Serial_ExchangePayloads(void *send, void *receive);
void Serial_PrepareSendPacket(void *payload);
u8 Serial_CollectReceivedPayloads(void *payload);
void Serial_StepBlockTransfer(void);
void Serial_EnableTransferTimer(void);
void Serial_DisableTransfer(void);
s32 Serial_StartSend(void *source, s32 size);
s32 Serial_StartReceive(void *destination);
void Serial_WaitForSend(void);
void Serial_WaitForReceive(void);
void Serial_WaitForTransfers(void);
s32 Serial_GetActiveTransfers(void);
void BattleLink_ResetTransferState(void);
void Serial_HandleTransferInterrupt(void);
void Serial_RemoveIrqHandlers(void);
u32 Serial_WaitForStatusMask(s32 mask);
extern struct SerialTransferState gSerialTransfer;
extern struct SerialWork gSerialWork;
extern u16 gSerialExchangeActive;
extern s32 gSerialSendSource;
// volatile: tagged where it steers, in Serial_StepBlockTransfer (tbs
// runtime.c)
extern volatile u16 gSerialSendSize;
extern s32 gSerialReceiveDest;
// volatile: tagged where it steers, in LinkBattle_ReceiveActions (tbs
// battle.c) and LinkLobby_ReceivePartyRecords (tbs lobby.c)
extern volatile u16 gSerialReceivedSize;
extern u8 gSerialPeerPayloads[4][24];
// volatile: tagged where it steers, in Serial_StepBlockTransfer (tbs
// runtime.c)
extern volatile u8 gSerialBlockNumber;
extern u8 gLinkExchangeState;
/// Link status, updated by the VBlank exchange handler.
/* FAKEMATCH: volatile makes every access to this cell a load or store of its
 * own; without it, in tbs-en, SaveMenu_Run: differ 221, 923 instructions for
 * 920, and Serial_WaitForStatusMask: differ 18, 21 for 23. */
extern volatile u16 gLinkStatus;

static inline s32 Serial_AddErrorFlag(s32 status) {
  if (gSerialWork.transferError != 0)
    status |= 0x1000;
  return status;
}

static __inline__ void Serial_ResetTransferState(void) {
  struct SerialTransferState *state;
  volatile u16 *ime;
  u32 saved;
  u32 zero;

  /* FAKEMATCH: the blocks preserve IME write order; the repeated state
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
  *(u8 *)&state->peerFlags = zero;
  *(u8 *)&state->flags = zero;
  gSerialReceivedSize = zero;
  *ime = saved;
}

#endif
