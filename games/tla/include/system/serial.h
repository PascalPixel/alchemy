#ifndef ALCHEMY_SERIAL_H
#define ALCHEMY_SERIAL_H

/// @file
/// The link cable's runtime: serial packets, registers and transfer state.

#include "edition.h"
#include "static_assert.h"
#include "types.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/io_write.h"
#include "system/irq.h"
#include "system/irq_lock.h"
#include "system/system.h"

/// A link cable packet: its packet number, the consoles not yet heard from,
/// its checksum and a 24-byte payload.
struct SerialPacket {
  u8 packetNumber; ///< SerialState's packet number when the packet was built
  /// The consoles heard from before but not this frame (receivedMask ^
  /// currentMask); a receiver complements it once the checksum is good.
  u8 missingMask;
  /// The complement of the sum of the packet's first 14 halfwords, taken with
  /// this field 0.
  u16 checksum;
  u8 payload[24];
  u8 reservedTail[4]; ///< past the 14 halfwords sent; never written
};

STATIC_ASSERT(sizeof(struct SerialPacket) == 32);
STATIC_ASSERT(offsetof(struct SerialPacket, packetNumber) == 0);
STATIC_ASSERT(offsetof(struct SerialPacket, missingMask) == 1);
STATIC_ASSERT(offsetof(struct SerialPacket, checksum) == 2);
STATIC_ASSERT(offsetof(struct SerialPacket, payload) == 4);
STATIC_ASSERT(offsetof(struct SerialPacket, reservedTail) == 28);

/// The serial data registers read as two words or four halfwords; no code
/// names this union.
union SerialDataRegisters {
  u32 words[2];
  u16 halfwords[4];
};

/// SerialState.phase.
enum SerialPhase {
  SERIAL_PHASE_SEARCHING = 0, ///< looking for the partner
  SERIAL_PHASE_CONNECTED = 1
};

/// SerialState.mode: how this console takes part in the link.
enum SerialMode {
  SERIAL_MODE_CHILD = 0,
  /// SIO_MULTI_SD alone of SIOCNT's SIO_START and SIO_MULTI_SD: this console
  /// armed timer 3 and clocks the transfers.
  SERIAL_MODE_CLOCKING = SIO_MULTI_SD
};

/// The link status bits Serial_ExchangePayloads returns.
enum LinkStatus {
  LINK_STATUS_HEARD_BOTH = 3,  ///< both consoles heard from this frame
  LINK_STATUS_CLOCKING = 0x80, ///< this console clocks the transfers
  /// The last transfer interrupt found SIOCNT's error bit (SIO_ERROR) set.
  LINK_STATUS_ERROR = 0x1000,
  /// SIOCNT's multiplayer id is 2 or more.
  LINK_STATUS_EXTRA_CONSOLE = 0x2000
};

/// The bytes of one packet buffer in SerialState.storage.
#define SERIAL_PACKET_BYTES 32

/// The link cable's runtime: its mode and phase, the consoles heard from, the
/// send position and the packet buffers the interrupt handler fills.
struct SerialState {
  /// An enum SerialMode; SERIAL_MODE_CLOCKING adds LINK_STATUS_CLOCKING to the
  /// link status.
  u8 mode;
  u8 phase;        ///< an enum SerialPhase
  u8 receivedMask; ///< the consoles heard from since the link began
  u8 currentMask;  ///< the consoles heard from this frame
  /// Each console's enum SerialChannelFlag bits, set by the transfer interrupt
  /// and cleared each frame.
  u8 channelFlags[4];
  /// Nonzero lets a new packet restart timer 3.
  u8 transferEnabled;
  /// SIOCNT's error bit (SIO_ERROR), which multiplayer mode sets when a
  /// transfer fails, copied by each transfer interrupt; nonzero adds
  /// LINK_STATUS_ERROR to the link status and refuses the packets heard
  /// meanwhile.
  u8 transferError;
  u8 reserved0a;
  /// The packet number, counted up each frame once connected.
  u8 packetNumber;
  u8 reserved0c[8];
  /// The halfword of sendBuffer[1] sent next; -1 between packets, when
  /// system_serial.c's SERIAL_IDLE_HALFWORD is sent and the two send buffers
  /// swap.
  s32 sendIndex;
  /// Each console's next halfword in its incomingBuffer; -1 after an idle
  /// system_serial.c's SERIAL_IDLE_HALFWORD.
  s32 receiveIndex[2];
  u8 reservedAfterReceiveIndex[8];
  /// The packet being built ([0]) and the packet being sent ([1]).
  u16 *sendBuffer[2];
  /// Each console's packet being received; two are used.
  u16 *incomingBuffer[4];
  /// Each console's last complete packet, swapped in by the interrupt.
  u16 *readyBuffer[4];
  /// The packets the frame code checks and copies, swapped with readyBuffer
  /// each frame.
  u16 *pendingBuffer[4];
  /// The buffers' memory: two 32-byte send buffers, then 96 bytes a console
  /// for its incoming, ready and pending packets.
  u8 storage[SERIAL_PACKET_BYTES * 8];
};

STATIC_ASSERT(sizeof(struct SerialState) == 352);
STATIC_ASSERT(offsetof(struct SerialState, mode) == 0);
STATIC_ASSERT(offsetof(struct SerialState, transferEnabled) == 8);
STATIC_ASSERT(offsetof(struct SerialState, sendIndex) == 20);
STATIC_ASSERT(offsetof(struct SerialState, storage) == 96);

/// SerialState.channelFlags: what the transfer interrupts saw of a console's
/// packets since the last frame.
enum SerialChannelFlag {
  SERIAL_CHANNEL_PACKET_COMPLETE = 1, ///< its fourteenth halfword arrived
  /// a transfer interrupt found SIOCNT's error bit (SIO_ERROR) set
  SERIAL_CHANNEL_ERROR = 2
};

/// SerialTransferState.flags: how far this console's receive has got.
enum SerialReceiveState {
  SERIAL_RECEIVE_IDLE = 0,
  SERIAL_RECEIVE_ACTIVE = 1,
  SERIAL_RECEIVE_DONE = 2 ///< the last block taken
};

/// SerialTransferState.peerFlags: the kind of block this console sends.
enum SerialSendState {
  SERIAL_SEND_IDLE = 0,
  SERIAL_SEND_MORE = 1, ///< a block with more to follow
  SERIAL_SEND_LAST = 2
};

/// SerialTransferState.active and gSerialBlockNumber hold a block number in
/// their low seven bits, counting up and wrapping; the receiver's, with bit 7
/// set, asks the sender to rewind to that block (Serial_StepBlockTransfer).
#define SERIAL_BLOCK_NUMBER_MASK 0x7f
#define SERIAL_BLOCK_REWIND 0x80 ///< bit 7: rewind to the block number
/// SerialTransferState.status's bit 7, set when a send or receive starts; the
/// receiver counts its blocks in the low bits (Serial_StepBlockTransfer).
#define SERIAL_STATUS_RUNNING 0x80

/// A console's block transfer state, exchanged with the partner each VBlank:
/// its block number, its status and flags, and a 20-byte block.
struct SerialTransferState {
  /// The number of the block in payload; as receiver, the one it
  /// expects with bit 7 set to ask for a resend; 1 when a transfer starts.
  u8 active;
  /// SERIAL_STATUS_RUNNING when started, plus 1 for a receive; the receiver
  /// counts blocks (status + 1, with bit 7) and sets 0 after the last.
  u8 status;
  u8 flags;       ///< the receive state, an enum SerialReceiveState
  u8 peerFlags;   ///< the send state, an enum SerialSendState
  u8 payload[20]; ///< one block of the data sent
};

STATIC_ASSERT(sizeof(struct SerialTransferState) == 24);
STATIC_ASSERT(offsetof(struct SerialTransferState, active) == 0);
STATIC_ASSERT(offsetof(struct SerialTransferState, status) == 1);
STATIC_ASSERT(offsetof(struct SerialTransferState, flags) == 2);
STATIC_ASSERT(offsetof(struct SerialTransferState, peerFlags) == 3);
STATIC_ASSERT(offsetof(struct SerialTransferState, payload) == 4);

#define SERIAL_RUNTIME (&gSerialState)

/// This console's block transfer state, exchanged with the partner's each
/// VBlank (Serial_ExchangePayloads).
extern struct SerialTransferState gSerialTransfer;
/// The link cable's runtime (SERIAL_RUNTIME), which Serial_Initialize clears.
extern struct SerialState gSerialState;
/// Nonzero while the VBlank handler exchanges packets with the partner each
/// frame; Serial_Initialize sets it.
extern u16 gSerialExchangeActive;
/// The data a block send reads from, 0 when none is under way
/// (Serial_StartSend).
extern s32 gSerialSendSource;
/// Bytes of the block send still to go: Serial_StartSend sets it and
/// Serial_StepBlockTransfer, which the VBlank handler runs, counts it down.
extern volatile u16 gSerialSendSize;
/// Where a block receive writes, 0 when none is under way
/// (Serial_StartReceive).
extern s32 gSerialReceiveDestination;
/// Bytes of the block receive taken so far: Serial_StepBlockTransfer, which the
/// VBlank handler runs, counts it up while the link code polls it.
extern volatile u16 gSerialReceivedSize;
/// Each console's SerialTransferState as last received, 24 bytes each.
extern struct SerialTransferState gSerialPeerPayloads[4];
/// The block this console sends or expects next (SERIAL_BLOCK_NUMBER_MASK):
/// Serial_StartSend and Serial_StartReceive reset it and
/// Serial_StepBlockTransfer, which the VBlank handler runs, steps it.
extern volatile u8 gSerialBlockNumber;
/// Cleared by Serial_Initialize and by LinkLobby_ExchangePartyRecords;
/// nothing reads it.
extern u8 gLinkExchangeState;
/// Link status, written by the VBlank exchange handler while
/// Serial_WaitForStatusMask and the link screens poll it.
extern volatile u16 gLinkStatus;

/// status, with LINK_STATUS_ERROR set when the link's last transfer failed.
static inline s32 Serial_AddErrorFlag(s32 status) {
  if (gSerialState.transferError != 0)
    status |= LINK_STATUS_ERROR;
  return status;
}

/// Starts a DMA 3 transfer; control_ is the unit count with the DMA_ flags
/// << 16.
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
void LinkBattle_ResetTransferState(void);
void Serial_HandleTransferInterrupt(void);
void Serial_RemoveIrqHandlers(void);

#endif
