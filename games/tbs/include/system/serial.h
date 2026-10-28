#ifndef ALCHEMY_SERIAL_H
#define ALCHEMY_SERIAL_H

/// @file
/// The link cable's runtime: serial packets, registers and transfer state.

#include "edition.h"
#include "types.h"
#include "static_assert.h"
#include "system.h"
#include "io_reg.h"
#include "dma.h"
#include "irq.h"
#include "dma.h"
#include "irq_lock.h"

/// A link cable packet: its packet number, the consoles not yet heard from,
/// its checksum and a 24-byte payload.
struct SerialPacket {
  u8 packetNumber; ///< SerialWork's packet number when the packet was built
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

/// The serial data registers read as two words or four halfwords; no TBS code
/// names this union.
union SerialDataRegisters {
  u32 words[2];
  u16 halfwords[4];
};

/// SerialWork.phase.
enum SerialPhase {
  SERIAL_PHASE_SEARCHING = 0, ///< looking for the partner
  SERIAL_PHASE_CONNECTED = 1
};

/// SerialWork.mode: how this console takes part in the link.
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

/// The link cable's runtime: its mode and phase, the consoles heard from, the
/// send position and the packet buffers the interrupt handler fills.
struct SerialWork {
  /// An enum SerialMode; SERIAL_MODE_CLOCKING adds LINK_STATUS_CLOCKING to the
  /// link status.
  u8 mode;
  u8 phase;        ///< an enum SerialPhase
  u8 receivedMask; ///< the consoles heard from since the link began
  u8 currentMask;  ///< the consoles heard from this frame
  /// Set by the interrupt for each console and cleared each frame.
  /// bits: 1 heard while transferError was set | 0 a packet completed
  u8 channelFlags[4];
  /// Nonzero lets a new packet restart timer 3.
  u8 transferEnabled;
  /// SIOCNT's error bit (SIO_ERROR), which multiplayer mode sets when a
  /// transfer fails, copied by each transfer interrupt; nonzero adds
  /// LINK_STATUS_ERROR to the link status and refuses the packets heard
  /// meanwhile.
  u8 transferError;
  u8 reserved0a; ///< not named by TBS code
  /// The packet number, counted up each frame once connected.
  u8 packetNumber;
  u8 reserved0c[8]; ///< not named by TBS code
  /// The halfword of sendBuffer[1] sent next; -1 between packets, when
  /// serial.c's SERIAL_IDLE_HALFWORD is sent and the two send buffers swap.
  s32 sendIndex;
  /// Each console's next halfword in its incomingBuffer; -1 after an idle
  /// serial.c's SERIAL_IDLE_HALFWORD.
  s32 receiveIndex[2];
  u8 reserved20[8]; ///< not named by TBS code
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
  u8 storage[0x100];
};

STATIC_ASSERT(sizeof(struct SerialWork) == 0x160);
STATIC_ASSERT(offsetof(struct SerialWork, mode) == 0);
STATIC_ASSERT(offsetof(struct SerialWork, transferEnabled) == 8);
STATIC_ASSERT(offsetof(struct SerialWork, sendIndex) == 0x14);
STATIC_ASSERT(offsetof(struct SerialWork, storage) == 0x60);

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

#define SERIAL_RUNTIME (&gSerialWork)

extern struct SerialTransferState gSerialTransfer;
extern struct SerialWork gSerialWork;
extern u16 gSerialExchangeActive;
extern s32 gSerialSendSource;
/// volatile: tagged where it steers, in Serial_StepBlockTransfer (tbs
/// runtime.c)
extern volatile u16 gSerialSendSize;
extern s32 gSerialReceiveDestination;
/// volatile: tagged where it steers, in LinkBattle_ReceiveActions (tbs
/// battle.c) and BattleArena_ReceivePartyRecords (tbs battle_arena.c)
extern volatile u16 gSerialReceivedSize;
extern u8 gSerialPeerPayloads[4][24];
/// volatile: tagged where it steers, in Serial_StepBlockTransfer (tbs
/// runtime.c)
extern volatile u8 gSerialBlockNumber;
extern u8 gLinkExchangeState;
/// Link status, updated by the VBlank exchange handler.
/* FAKEMATCH(volatile): each mask test loads the status on its own; as plain
 * u16, 155 of 920 instructions differ in tbs-en (FileScreen_Run; 14 of 23 in
 * Serial_WaitForStatusMask). */
extern volatile u16 gLinkStatus;

static inline s32 Serial_AddErrorFlag(s32 status) {
  if (gSerialWork.transferError != 0)
    status |= LINK_STATUS_ERROR;
  return status;
}

static __inline__ void Serial_ResetTransferState(void) {
  struct SerialTransferState *state;
  volatile u16 *ime;
  u32 saved;
  u32 zero;

  state = &gSerialTransfer;
  ime = &REG_IME;
  IME_SAVE(ime, saved);
  IME_DISABLE(ime);
  /* FAKEMATCH(one-pass): zero and state set in a once-only block; as plain
   * statements, 27 of 62 instructions differ in tbs-en
   * (BattleArena_ExchangePartyRecords). */
  do {
    zero = 0;
    /* FAKEMATCH(dead): state set again; as the one assignment above, 16 of 62
     * instructions differ in tbs-en (BattleArena_ExchangePartyRecords). */
    state = &gSerialTransfer;
  } while (0);
  state->status = SERIAL_STATUS_RUNNING;
  gSerialSendSource = zero;
  gSerialSendSize = zero;
  gSerialReceiveDestination = zero;
  *(u8 *)&state->peerFlags = zero;
  *(u8 *)&state->flags = zero;
  gSerialReceivedSize = zero;
  *ime = saved;
}

#endif
