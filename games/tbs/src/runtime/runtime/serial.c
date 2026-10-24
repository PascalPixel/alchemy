/// @file
/// The link cable: payload exchange, block transfers and the link test.
// Part of runtime.c.

#include "../runtime_internal.h"

// The link cable. Two consoles trade a 24-byte payload each VBlank in
// multiplayer mode; the parent clocks the transfers from timer 3, a halfword
// at a time. Bulk data rides on that exchange in 20-byte blocks (the
// BeginTransfer functions and Serial_StepBlockTransfer).

enum {
  /// A packet: sequence, missing mask, checksum and the 24-byte payload.
  SERIAL_PACKET_HALFWORDS = 14,
  SERIAL_PAYLOAD_BYTES = 24, ///< one console's payload each frame
  SERIAL_BLOCK_BYTES = 20,   ///< bulk data carried in each payload
  /// About 10,000 seconds, nearly three hours.
  SERIAL_TRANSFER_TIMEOUT_FRAMES = 600000,
  SERIAL_IDLE_HALFWORD = 0xfefe, ///< sent and seen between packets
  /// Timer 3's reload: 13981 cycles from one halfword transfer to the next,
  /// about 20 a frame.
  SERIAL_TIMER_RELOAD = 0xc963
};

/// Resets the serial port into multiplayer mode, installs the serial and
/// timer 3 interrupt handlers, clears gSerialWork and points its packet
/// buffers into its storage, clears the bulk transfer state and sets
/// gSerialExchangeActive, which starts the per-frame exchange. Called by the
/// Battle Arena, the file menu's link transfer and the link test, and through
/// the far-call table.
void Serial_Initialize(void) {
  u32 interruptEnable;
  volatile u16 *ime;
  s32 index;
  struct SerialWork *state;

  // IRQ_LOCK written out, with the IME address set inside the save, which
  // reads IME before the frame is made (as IRQ_LOCK, 1 of 123 instructions
  // differs).
  IME_SAVE((ime = &REG_IME), interruptEnable);
  IME_DISABLE(ime);
  // Replace both interrupt handlers before configuring multiplayer transfers.
  Irq_SetHandler(IRQ_SERIAL, 0, Serial_HandleTransferInterrupt);
  Irq_SetHandler(IRQ_TIMER3, 0, Serial_HandleTransferInterrupt);
  REG_IME = 0;
  REG_IE &= (u16)~(INTR_FLAG_SERIAL | INTR_FLAG_TIMER3);
  if ((REG_IF & INTR_FLAG_SERIAL) != 0)
    REG_IF = INTR_FLAG_SERIAL;
  if ((REG_IF & INTR_FLAG_TIMER3) != 0)
    REG_IF = INTR_FLAG_TIMER3;
  REG_RCNT = RCNT_GPIO_MODE;
  REG_RCNT = RCNT_SIO_MODE;
  REG_SIOCNT = SIO_32BIT_MODE;
  REG_RCNT = RCNT_SIO_MODE;
  REG_SIOCNT = SIO_MULTI_MODE;
  REG_SIOCNT16 = REG_SIOCNT16 | SIO_INTR_ENABLE | SIO_115200_BPS;
  state = &gSerialWork;
  REG_IME = 1;
  Dma_Fill32(0, state, sizeof(*state));
  state->sendIndex = -1;
  state->sendBuffer[0] = (u16 *)(state->storage + 0);
  state->sendBuffer[1] = (u16 *)(state->storage + 32);
  // Each console has separate incoming, completed and pending packet buffers.
  for (index = 0; index <= 1; index++) {
    state->incomingBuffer[index] = (u16 *)(state->storage + 64 + index * 96);
    state->readyBuffer[index] = (u16 *)(state->storage + 96 + index * 96);
    state->pendingBuffer[index] = (u16 *)(state->storage + 128 + index * 96);
  }
  REG_IME = 0;
  REG_IE |= INTR_FLAG_SERIAL;
  REG_IME = 1;
  gSerialExchangeActive = 1;
  gLinkExchangeState = 0;
  gSerialSendSource = 0;
  gSerialSendSize = 0;
  gSerialReceiveDestination = 0;
  gSerialReceivedSize = 0;
  LinkBattle_ResetTransferState();
  IRQ_UNLOCK(interruptEnable);
}

/// Lets the timer start transfers again once a link mode is set, by setting
/// gSerialWork.transferEnabled. Nothing in ☀️ calls it; WaitFrames does the
/// same inline.
void Serial_EnableTransferTimer(void) {
  struct SerialWork *state = &gSerialWork;

  if (state->mode != 0)
    state->transferEnabled = 1;
}

/// Disables the serial and timer 3 interrupts, stops the transfer timer and
/// clears gSerialWork.transferEnabled. Nothing in ☀️ calls it.
void Serial_DisableTransfer(void) {
  struct SerialWork *state;

  REG_IME = 0;
  REG_IE &= (u16)~(INTR_FLAG_SERIAL | INTR_FLAG_TIMER3);
  REG_IME = 1;
  REG_SIOCNT16 = SIO_MULTI_MODE | SIO_115200_BPS;
  // Timer 3 stopped, with its reload set
  REG_TM3CNT = SERIAL_TIMER_RELOAD;
  REG_IF = INTR_FLAG_SERIAL | INTR_FLAG_TIMER3;
  state = &gSerialWork;
  state->transferEnabled = 0;
}

/// Looks for the link partner and, once connected, does one exchange (as
/// Serial_ExchangePayloads). When SIOCNT shows the parent's id with
/// SIO_MULTI_SD set and SIO_MULTI_SI and SIO_START clear, and no packet is
/// being sent, this console arms timer 3 to clock the transfers and so acts as
/// the parent. Called by the Tret tree branch scene for its own exchange.
/// Returns the link status (see Serial_ExchangePayloads).
s32 Serial_PollAndTransfer(void *send, void *receive) {
  volatile u32 *siocnt;
  u32 control;
  s32 status;
  s32 result;

  siocnt = &REG_SIOCNT;
  control = *siocnt;
  switch (gSerialWork.phase) {
  case SERIAL_PHASE_SEARCHING: {
    u8 multiplayerId = control & SIO_ID;
    u8 mode;

    // The parent waits here until SIO_MULTI_SD is set with SIO_START clear;
    // a child connects at once.
    if (multiplayerId == 0 &&
        (mode = control & (SIO_START | SIO_MULTI_SD)) == SERIAL_MODE_CLOCKING) {
      u8 serialError = control & SIO_MULTI_SI;

      if (serialError == 0 && gSerialWork.sendIndex == -1) {
        u32 ie;

        Io_Write16(0, &REG_IME);
        ie = REG_IE;
        Io_Write16((ie & ~INTR_FLAG_SERIAL) | INTR_FLAG_TIMER3, &REG_IE);
        Io_Write16(1, &REG_IME);
        // SIOCNT's high byte: clear SIO_INTR_ENABLE, as timer 3 now clocks
        // the transfers
        *((volatile s8 *)siocnt + 1) &= ~0x40;
        REG_IF = INTR_FLAG_SERIAL | INTR_FLAG_TIMER3;
        REG_TM3CNT = SERIAL_TIMER_RELOAD;
        gSerialWork.mode = mode;
      }
    } else if (multiplayerId == 0) {
      break;
    }
    gSerialWork.phase = SERIAL_PHASE_CONNECTED;
  }
    // Fall through.
  case SERIAL_PHASE_CONNECTED:
    Serial_CollectReceivedPayloads(receive);
    Serial_PrepareSendPacket(send);
    break;
  }

  gSerialWork.sequence++;
  status = gSerialWork.currentMask | (gSerialWork.receivedMask << 8);
  if (gSerialWork.mode == SERIAL_MODE_CLOCKING)
    status |= LINK_STATUS_CLOCKING;
  result = Serial_AddErrorFlag(status);
  if (((control << 26) >> 30) > 1)
    result |= LINK_STATUS_EXTRA_CONSOLE;
  return result;
}

/// As Serial_PollAndTransfer, without exchanging data: WaitFrames calls
/// it once a frame while gSerialExchangeActive is set, and the VBlank handler
/// does the exchange.
s32 Serial_PollStatus(void) {
  volatile u32 *siocnt;
  u32 control;
  s32 status;
  s32 result;

  siocnt = &REG_SIOCNT;
  control = *siocnt;
  if (gSerialWork.phase == SERIAL_PHASE_SEARCHING) {
    u8 mode = control & (SIO_START | SIO_MULTI_SD);

    if (mode == SERIAL_MODE_CLOCKING) {
      u8 serialError = control & SIO_MULTI_SI;

      if (serialError == 0 && gSerialWork.sendIndex == -1) {
        u32 ie;

        Io_Write16(0, &REG_IME);
        ie = REG_IE;
        Io_Write16((ie & ~INTR_FLAG_SERIAL) | INTR_FLAG_TIMER3, &REG_IE);
        Io_Write16(1, &REG_IME);
        // SIOCNT's high byte: clear SIO_INTR_ENABLE, as timer 3 now clocks
        // the transfers
        *((volatile s8 *)siocnt + 1) &= ~0x40;
        REG_IF = INTR_FLAG_SERIAL | INTR_FLAG_TIMER3;
        REG_TM3CNT = SERIAL_TIMER_RELOAD;
        gSerialWork.mode = mode;
      }
      gSerialWork.phase = SERIAL_PHASE_CONNECTED;
    }
    gSerialWork.sequence++;
  }

  status = gSerialWork.currentMask | (gSerialWork.receivedMask << 8);
  if (gSerialWork.mode == SERIAL_MODE_CLOCKING)
    status |= LINK_STATUS_CLOCKING;
  result = Serial_AddErrorFlag(status);
  if (((control << 26) >> 30) > 1)
    result |= LINK_STATUS_EXTRA_CONSOLE;
  return result;
}

/// Once connected (SERIAL_PHASE_CONNECTED), gathers the received payloads into
/// `receive` and builds the next packet from `send`. Called by the VBlank
/// handler while gSerialExchangeActive is set, which stores the result in
/// gLinkStatus. Returns the link status:
///   bits 0 and 1 : consoles 0 and 1 heard from this frame
///   0x80         : this console clocks the transfers (LINK_STATUS_CLOCKING)
///   bits 8 and 9 : consoles 0 and 1 heard from since the link began
///   0x1000       : the last transfer interrupt found SIOCNT's error bit
///                  set (LINK_STATUS_ERROR)
///   0x2000       : SIOCNT's multiplayer id is 2 or more
/// So LINK_STATUS_HEARD_BOTH is what the block transfer and
/// Serial_WaitForStatusMask(LINK_STATUS_HEARD_BOTH) wait for.
s32 Serial_ExchangePayloads(void *send, void *receive) {
  volatile u32 *siocnt;
  u32 control;
  s32 status;
  s32 result;

  siocnt = &REG_SIOCNT;
  control = *siocnt;
  if (gSerialWork.phase == SERIAL_PHASE_CONNECTED) {
    Serial_CollectReceivedPayloads(receive);
    Serial_PrepareSendPacket(send);
    gSerialWork.sequence++;
  }
  status = gSerialWork.currentMask | (gSerialWork.receivedMask << 8);
  if (gSerialWork.mode == SERIAL_MODE_CLOCKING)
    status |= LINK_STATUS_CLOCKING;
  result = Serial_AddErrorFlag(status);
  // SIOCNT bits 4 and 5, the multiplayer id
  if (((control << 26) >> 30) > 1)
    result |= LINK_STATUS_EXTRA_CONSOLE;
  return result;
}

/// Builds the packet to send in sendBuffer[0]: [0] the sequence number, [1]
/// the consoles not yet heard from, [2] the checksum and [4] the 24-byte
/// payload. The checksum is the complement of the sum of the first 28 bytes
/// as halfwords. Then restarts the send position and the timer. Called by
/// both exchanges.
void Serial_PrepareSendPacket(void *payload) {
  /* FAKEMATCH(volatile): sr is volatile, so sendBuffer[0] is loaded again for
   * the checksum; as a plain pointer, 4 of 48 instructions differ in
   * tbs-en. */
  volatile struct SerialWork *sr;
  u32 i;
  u32 sum;

  sr = SERIAL_RUNTIME;
  {
    u8 *pkt;
    u8 cur;
    u8 recv;

    pkt = (u8 *)sr->sendBuffer[0];
    pkt[0] = sr->sequence;
    cur = sr->currentMask;
    recv = sr->receivedMask;
    sum = 0;
    pkt[1] = recv ^ cur;
    *(u16 *)(pkt + 2) = sum;
    Dma_Set(payload, pkt + 4, DMA_COPY_WORDS(6), REG_DMA3);
  }
  {
    u16 *pkt;
    u32 v;

    pkt = sr->sendBuffer[0];
    for (i = 0; i < SERIAL_PACKET_HALFWORDS; i++) {
      v = *pkt;
      pkt++;
      sum += v;
    }
  }
  {
    u32 inv;
    struct SerialPacket *pkt;

    inv = ~sum;
    pkt = (struct SerialPacket *)sr->sendBuffer[0];
    pkt->checksum = inv;
  }

  // Stop timer 3, rewind the send position and start the timer again
  if (sr->mode != 0)
    REG_TM3CNT_H = 0;
  sr->sendIndex = -1;
  if (sr->mode != 0 && sr->transferEnabled != 0)
    REG_TM3CNT_H = TIMER_ENABLE | TIMER_INTR_ENABLE;
}

/// Swaps in the packets the interrupt handler finished, then copies the
/// payload of each one whose checksum is good to `payload` (24 bytes per
/// console), and records the consoles heard from in currentMask and
/// receivedMask. Called by both exchanges. Returns the mask of consoles heard
/// from this frame.
u8 Serial_CollectReceivedPayloads(void *payload) {
  u8 flags[8];
  s32 i;
  u32 k;
  u16 *packet;
  u16 *swap;
  s32 sum;

  *(u32 *)&flags[4] = 0;
  // Swap completed buffers with interrupts masked, then validate outside it.
  Io_Write16(0, &REG_IME);
  for (i = 0; i <= 3; i++) {
    swap = gSerialWork.pendingBuffer[i];
    gSerialWork.pendingBuffer[i] = gSerialWork.readyBuffer[i];
    gSerialWork.readyBuffer[i] = swap;
  }
  *(u32 *)flags = *(u32 *)gSerialWork.channelFlags;
  *(u32 *)gSerialWork.channelFlags = 0;
  Io_Write16(1, &REG_IME);
  gSerialWork.currentMask = 0;
  for (i = 0; i <= 1; i++) {
    packet = gSerialWork.pendingBuffer[i];
    sum = 0;
    for (k = 0; k < SERIAL_PACKET_HALFWORDS; k++)
      sum += packet[k];
    // A good packet's halfwords, its complemented checksum included, sum to
    // 0xffff. Its payload is taken only when the packet completed since the
    // last frame with flag bit 1 (transferError at a transfer) never set.
    if (flags[i] == 1 && (s16)sum == -1) {
      Dma_Set(gSerialWork.pendingBuffer[i] + 2,
              (u8 *)payload + i * SERIAL_PAYLOAD_BYTES, DMA_COPY_WORDS(6),
              REG_DMA3);
      gSerialWork.currentMask |= 1 << i;
    }
    // Complementing a halfword spoils the sum, so a buffer that comes round
    // again unchanged is never taken twice
    if ((s16)sum == -1)
      gSerialWork.pendingBuffer[i][1] = ~gSerialWork.pendingBuffer[i][1];
  }
  gSerialWork.receivedMask |= gSerialWork.currentMask;
  return gSerialWork.currentMask;
}

/// The serial and timer 3 interrupt, installed by Serial_Initialize:
/// each one moves a halfword. It sends the next halfword of the outgoing
/// packet (0xfefe between packets) and stores the halfword received from each
/// console; a console's fourteenth halfword completes its packet.
void Serial_HandleTransferInterrupt(void) {
  // The receive channels share one backedge; a do/while for each adds 50 bytes
  // in tbs-en.
  struct SerialWork *sendState;
  struct SerialWork *receiveState;
  struct SerialWork *tailState;
  u16 recv[4];
  volatile u32 *sioControl;
  s32 channel;

  // The four consoles' halfwords, SIOMULTI0 to 3.
  // UB: the u16 array is written through one doubleword store
#ifdef UBFIX
  {
    volatile u16 *multi = (volatile u16 *)REG_SIODATA32;

    recv[0] = multi[0];
    recv[1] = multi[1];
    recv[2] = multi[2];
    recv[3] = multi[3];
  }
#else
  *(u64 *)recv = *(volatile u64 *)REG_SIODATA32;
#endif
  sioControl = &REG_SIOCNT;
  sendState = &gSerialWork;
  // SIOCNT's error bit (SIO_ERROR): a packet heard while it is set is refused
  // (flag bit 1, below)
  sendState->transferError = (*sioControl << 25) >> 31;

  if (sendState->sendIndex == -1) {
    u32 idleWord = SERIAL_IDLE_HALFWORD;
    u16 *swap;

    ((volatile u16 *)sioControl)[1] = idleWord;
    swap = sendState->sendBuffer[1];
    sendState->sendBuffer[1] = sendState->sendBuffer[0];
    sendState->sendBuffer[0] = swap;
  } else if (sendState->sendIndex >= 0) {
    ((volatile u16 *)sioControl)[1] =
        sendState->sendBuffer[1][sendState->sendIndex];
  }
  receiveState = &gSerialWork;
  if (receiveState->sendIndex <= SERIAL_PACKET_HALFWORDS)
    receiveState->sendIndex++;

  channel = 0;
receive_loop:
  // Hand complete packets to the frame code without overwriting its buffers.
  {
    s32 receiveIndex;
    u16 *incoming;

    if (recv[channel] == SERIAL_IDLE_HALFWORD &&
        receiveState->receiveIndex[channel] >= SERIAL_PACKET_HALFWORDS) {
      receiveState->receiveIndex[channel] = -1;
    } else {
      receiveIndex = receiveState->receiveIndex[channel];
      incoming = receiveState->incomingBuffer[channel];
      incoming[receiveIndex] = recv[channel];
      if (receiveIndex == SERIAL_PACKET_HALFWORDS - 1) {
        u16 *swap = receiveState->readyBuffer[channel];

        receiveState->readyBuffer[channel] = incoming;
        receiveState->incomingBuffer[channel] = swap;
        receiveState->channelFlags[channel] |= 1;
      }
    }
  }
  tailState = &gSerialWork;
  if (tailState->transferError != 0)
    tailState->channelFlags[channel] |= 2;
  if (tailState->receiveIndex[channel] <= SERIAL_PACKET_HALFWORDS)
    tailState->receiveIndex[channel]++;
  channel++;
  receiveState = tailState;
  if (channel <= 1)
    goto receive_loop;

  // The console that clocks the transfers starts the next one, and restarts
  // timer 3 so its count reloads
  if (tailState->mode == SERIAL_MODE_CLOCKING) {
    REG_TM3CNT_H = 0;
    REG_SIOCNT16 |= SIO_START;
    REG_TM3CNT_H = TIMER_ENABLE | TIMER_INTR_ENABLE;
  }
}

/// Clears gSerialExchangeActive, which stops the per-frame exchange, and
/// removes the link interrupt handlers. Called when the Battle Arena, the file
/// menu's transfer or a link battle ends.
void Serial_RemoveIrqHandlers(void) {
  gSerialExchangeActive = 0;
  Irq_SetHandler(IRQ_SERIAL, 0, 0);
  Irq_SetHandler(IRQ_TIMER3, 0, 0);
}

/// Waits a frame at a time until every status bit in `mask` is set in
/// gLinkStatus, then returns this console's multiplayer id. Called by the
/// link test and through the far-call table.
u32 Serial_WaitForStatusMask(s32 mask) {
  if ((mask & gLinkStatus) != mask) {
    do {
      WaitFrames(1);
    } while ((mask & gLinkStatus) != mask);
  }
  return (REG_SIOCNT << 0x1A) >> 0x1E;
}

/// Starts sending `size` bytes from `source` to the partner in 20-byte blocks:
/// sets the send source, size and block sequence that
/// Serial_StepBlockTransfer works from. Called by the link battle,
/// the Battle Arena and the file menu's transfer. Returns -1 when a send is
/// already running, 0 otherwise.
///
/// `size` must be a multiple of 20: it falls by 20 a block, and the send ends
/// only when it reaches exactly 0. `source` is read a block each VBlank, and
/// may be resent from after a rewind, so it must stay unchanged until
/// gSerialSendSource is 0 again.
s32 Serial_StartSend(void *source, s32 size) {
  struct SerialTransferState *state = &gSerialTransfer;
  u32 savedInterruptMaster;

  if (gSerialSendSource != 0)
    return -1;
  IRQ_LOCK(savedInterruptMaster);
  state->status = 0x80;
  gSerialSendSource = (s32)source;
  gSerialSendSize = size;
  gSerialBlockSequence = 0;
  state->active = 1;
  IRQ_UNLOCK(savedInterruptMaster);
  return 0;
}

/// Starts receiving the partner's blocks into `destination`: sets the receive
/// destination and clears the received size. Called by the same code as
/// Serial_StartSend. Returns -1 when a receive is already running, 0
/// otherwise. Each block is stored whole, so `destination` needs room for
/// the partner's whole send; the receive ends at the partner's last block,
/// whatever the room.
s32 Serial_StartReceive(void *destination) {
  struct SerialTransferState *state = &gSerialTransfer;
  u32 savedInterruptMaster;

  if (gSerialReceiveDestination != 0)
    return -1;
  IRQ_LOCK(savedInterruptMaster);
  state->status = 0x81;
  gSerialReceiveDestination = (s32)destination;
  gSerialReceivedSize = 0;
  state->active = 1;
  gSerialBlockSequence = 0;
  IRQ_UNLOCK(savedInterruptMaster);
  return 0;
}

/// Each waits a frame at a time until `pending` is 0, giving up after 600000
/// frames: the send, then the receive.
#define SERIAL_DEFINE_WAIT(name, pending)                                      \
  void name(void) {                                                            \
    u32 count = 0;                                                             \
                                                                               \
    if (pending != 0) {                                                        \
      do {                                                                     \
        WaitFrames(1);                                                         \
        count++;                                                               \
      } while (count < SERIAL_TRANSFER_TIMEOUT_FRAMES && pending != 0);        \
    }                                                                          \
  }

SERIAL_DEFINE_WAIT(Serial_WaitForSend, gSerialSendSource)
SERIAL_DEFINE_WAIT(Serial_WaitForReceive, gSerialReceiveDestination)

/// Waits until both the send and the receive have finished, giving up after
/// 600000 frames. Reached through the far-call table.
void Serial_WaitForTransfers(void) {
  u32 count;

  count = 0;
  while (gSerialSendSource != 0 || gSerialReceiveDestination != 0) {
    WaitFrames(1);
    count++;
    if (count >= SERIAL_TRANSFER_TIMEOUT_FRAMES)
      return;
  }
}

/// Returns bit 0 while a send runs and bit 1 while a receive runs. The link
/// battle and the Battle Arena poll it between frames.
s32 Serial_GetActiveTransfers(void) {
  s32 flags;

  flags = 0;
  if (gSerialSendSource != 0) {
    flags = 1;
  }
  if (gSerialReceiveDestination != 0) {
    flags |= 2;
  }
  return flags;
}

void LinkBattle_ResetTransferState(void) { Serial_ResetTransferState(); }

struct SerialHalfword {
  u16 v;
};

/// Runs each VBlank after the exchange, called by System_VBlankHandler: moves
/// the bulk transfer on by one 20-byte block each way. The receiver
/// acknowledges each block's sequence number; when the sender sees an
/// acknowledgement behind its own count it rewinds and sends the missed
/// blocks again. A finished send or receive clears gSerialSendSource or
/// gSerialReceiveDestination.
void Serial_StepBlockTransfer(void) {
  /* FAKEMATCH(volatile): gSerialSendSize and gSerialBlockSequence are volatile
   * (serial_runtime.h), so each access is its own load or store; as plain
   * globals, 99 of 257 instructions differ in tbs-en. */
  struct SerialTransferState *peer;
  u32 id;
  struct SerialTransferState *own;
  u32 gap;
  /* FAKEMATCH(temporary): the 1 kept in a halfword record, loaded from the
   * pool into r8 at entry; as the constant 1 at both stores, 44 of 257
   * instructions differ in tbs-en. */
  struct SerialHalfword one;

  // The partner's payload: of multiplayer ids 0 and 1, the one not ours
  id = ~((REG_SIOCNT << 26) >> 30) & 1;
  one.v = 1;
  own = &gSerialTransfer;
  peer = (struct SerialTransferState *)gSerialPeerPayloads[id];
  // Both consoles must have been heard from this frame
  if ((gLinkStatus & LINK_STATUS_HEARD_BOTH) != LINK_STATUS_HEARD_BOTH)
    return;
  // Receiving advances only when the peer presents the expected sequence.
  // peerFlags is the sender's block kind, an enum SerialSendState.
  {
    s32 *receive = &gSerialReceiveDestination;
    s32 dest = *receive;
    if (dest != 0) {
      if (own->flags == SERIAL_RECEIVE_ACTIVE &&
          (u8)(peer->peerFlags - SERIAL_SEND_MORE) <=
              SERIAL_SEND_LAST - SERIAL_SEND_MORE) {
        if (peer->active == (gSerialBlockSequence & 0x7f)) {
          own->active = 0;
          switch (peer->peerFlags) {
          case SERIAL_SEND_MORE:
            // A block with more to come: store it and move on
            Dma_Set(peer->payload, (void *)dest, DMA_COPY_WORDS(5), REG_DMA3);
            *receive += SERIAL_BLOCK_BYTES;
            gSerialReceivedSize += SERIAL_BLOCK_BYTES;
            own->status = (own->status + 1) | 0x80;
            break;
          case SERIAL_SEND_LAST:
            // The last block: store it and report the receive finished
            Dma_Set(peer->payload, (void *)dest, DMA_COPY_WORDS(5), REG_DMA3);
            gSerialReceivedSize += SERIAL_BLOCK_BYTES;
            own->flags = SERIAL_RECEIVE_DONE;
            own->status = 0;
            own->active = one.v;
            break;
          }
          gSerialBlockSequence = (gSerialBlockSequence + 1) & 0x7f;
        } else if (gSerialBlockSequence & 0x80) {
          // Out of step and already asked: bit 7 of active is the request to
          // rewind to the sequence in its low bits
          if (own->active & 0x80) {
            own->active = one.v;
          } else if (own->active == 1) {
            own->active = 0;
            gSerialBlockSequence &= 0x7f;
          }
        } else {
          own->active = gSerialBlockSequence | 0x80;
          gSerialBlockSequence |= 0x80;
        }
      } else {
        own->active = 0;
      }
    }
  }
  {
    s32 *send = &gSerialSendSource;
    s32 src = *send;
    // A peer behind our sequence requests a rewind before the next block.
    if (src != 0) {
      if (peer->flags == SERIAL_RECEIVE_ACTIVE) {
        if (peer->active & 0x80) {
          gap = (gSerialBlockSequence - peer->active) & 0x7f;
          *send = src - gap * SERIAL_BLOCK_BYTES;
          gSerialSendSize += gap * SERIAL_BLOCK_BYTES;
          gSerialBlockSequence -= gap;
          gSerialBlockSequence &= 0x7f;
        }
        // Send the next block, marked as the last when it empties the size
        if (gSerialSendSize != 0) {
          Dma_Set((void *)*send, own->payload, DMA_COPY_WORDS(5), REG_DMA3);
          gSerialSendSize += -SERIAL_BLOCK_BYTES;
          if (gSerialSendSize != 0) {
            own->peerFlags = SERIAL_SEND_MORE;
            own->active = gSerialBlockSequence & 0x7f;
            *send += SERIAL_BLOCK_BYTES;
          } else {
            own->peerFlags = SERIAL_SEND_LAST;
            own->active = gSerialBlockSequence & 0x7f;
            *send += SERIAL_BLOCK_BYTES;
          }
          gSerialBlockSequence = (gSerialBlockSequence + 1) & 0x7f;
        }
      }
      // The partner has stored the last block: the send is finished
      if (own->peerFlags == SERIAL_SEND_LAST &&
          peer->flags == SERIAL_RECEIVE_DONE) {
        gSerialSendSource = 0;
        own->peerFlags = SERIAL_SEND_IDLE;
        own->active = 1;
      }
    }
  }
  // flags tells the partner this side is receiving or has received the last
  // block; the receive ends once the partner has seen that
  if (own->flags == SERIAL_RECEIVE_DONE) {
    if (peer->peerFlags != SERIAL_SEND_LAST) {
      gSerialReceiveDestination = 0;
      own->flags = SERIAL_RECEIVE_IDLE;
    }
  } else {
    own->flags = SERIAL_RECEIVE_IDLE;
    if (gSerialReceiveDestination != 0)
      own->flags = SERIAL_RECEIVE_ACTIVE;
  }
}

extern const u8 Rom_Start[];

/// An empty routine the link test calls to waste time.
void SerialTest_Delay(void) {}

/// A link cable test left in the game: it shows a row of tiles, receives blocks
/// into the map grid cell buffer and shows them as tiles, and sends a part of
/// the ROM when A or B is held. It never returns. Nothing in ☀️ reaches it.
void SerialTest_Run(void) {
  // An outer receive loop with an inner completion break changes 9 bytes in
  // tbs-en: the key-address load is hoisted and the restart retargeted.
  u16 *tile;
  s32 value;
  s32 n;
  s32 tick;

  Sound_PlaySongFar(MUS_PALACE);
  Serial_Initialize();
  // Row 16, columns 0 to 19: tiles 0x80 to 0x93 in palette 15, the 20 tiles
  // at 0x06001000 that the received 640 bytes are copied to
  n = 19;
  tile = (u16 *)BG_SCREEN_BLOCK(4) + 16 * 32 + 19;
  value = (s16)0xf093;
  do {
    n--;
    *tile = value;
    tile--;
    value--;
  } while (n >= 0);
  Cpu_Fill32(0, gMapGridCellBuffer, 0x400);
  Serial_WaitForStatusMask(LINK_STATUS_HEARD_BOTH);
restart:
  Serial_StartReceive(gMapGridCellBuffer);
  for (;;) {
    // 0x280 bytes: 32 blocks, 20 tiles
    if (gKeysHeld & KEY_A)
      Serial_StartSend((void *)Cart_Restart, 0x280);
    if (gKeysHeld & KEY_B)
      Serial_StartSend((void *)(Rom_Start + 0x1000), 0x280);
    if (gKeysHeld & KEY_START) {
      tick = 9999;
      do {
        tick--;
        SerialTest_Delay();
      } while (tick >= 0);
    }
    if (gSerialReceiveDestination == 0) {
      Dma_Set(gMapGridCellBuffer, (void *)0x06001000, DMA_COPY_WORDS(0xa0),
              REG_DMA3);
      goto restart;
    }
    WaitFrames(1);
  }
}

#undef SERIAL_DEFINE_WAIT
