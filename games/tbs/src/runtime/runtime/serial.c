/// @file
/// The link cable: payload exchange, block transfers and the link test.
/// Part of runtime.c, the runtime object, compiled only through it.

#include "../runtime_internal.h"

// The link cable. Two consoles trade a 24-byte payload each VBlank in
// multiplayer mode; the parent clocks the transfers from timer 3, a halfword
// at a time. Bulk data rides on that exchange in 20-byte blocks (the
// BeginTransfer functions and SerialRuntime_StepBlockTransfer).

enum {
  SERIAL_PACKET_HALFWORDS = 14,
  SERIAL_PAYLOAD_BYTES = 24,
  SERIAL_BLOCK_BYTES = 20,
  SERIAL_TRANSFER_TIMEOUT_FRAMES = 600000,
  SERIAL_IDLE_HALFWORD = 0xfefe
};

/// Resets the serial port into multiplayer mode, installs the serial and
/// timer 3 interrupt handlers, sets up the packet buffers and starts the
/// per-frame exchange.
void SerialRuntime_Initialize(void) {
  u32 interruptEnable;
  volatile u16 *ime;
  s32 index;
  struct SerialRuntime *state;

  /* FAKEMATCH: IRQ_LOCK written out with the IME address set inside the save
   * statement, so the IME load comes before sub sp, #4; as IRQ_LOCK, 2 of 123
   * instructions differ in tbs-en. */
  IME_SAVE((ime = &REG_IME), interruptEnable);
  IME_DISABLE(ime);
  // Replace both interrupt handlers before configuring multiplayer transfers.
  Runtime_SetIrqHandler(IRQ_SERIAL, 0, SerialRuntime_HandleTransferInterrupt);
  Runtime_SetIrqHandler(IRQ_TIMER3, 0, SerialRuntime_HandleTransferInterrupt);
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
  state = &gSerialRuntime;
  REG_IME = 1;
  /* FAKEMATCH: Dma_Fill32's fill word is volatile, so the zero is stored
   * through its address (mov r0, sp; str r3, [r0]); as a plain u32, 6
   * instructions differ in tbs-en. */
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
  gSerialReceiveDest = 0;
  gSerialReceivedSize = 0;
  BattleLink_ResetTransferState();
  IRQ_UNLOCK(interruptEnable);
}

/// Lets the timer start transfers again once a link mode is set.
void SerialRuntime_EnableTransferTimer(void) {
  struct SerialRuntime *state = &gSerialRuntime;

  if (state->mode != 0)
    state->transferEnabled = 1;
}

/// Disables the serial and timer interrupts and stops the transfer timer.
void SerialRuntime_DisableTransfer(void) {
  struct SerialRuntime *state;

  REG_IME = 0;
  REG_IE &= (u16)~(INTR_FLAG_SERIAL | INTR_FLAG_TIMER3);
  REG_IME = 1;
  REG_SIOCNT16 = SIO_MULTI_MODE | SIO_115200_BPS;
  REG_TM3CNT = 0x0000c963;
  REG_IF = INTR_FLAG_SERIAL | INTR_FLAG_TIMER3;
  state = &gSerialRuntime;
  state->transferEnabled = 0;
}

/// Looks for the link partner and, once connected, does one exchange (as
/// SerialRuntime_ExchangePayloads). When this console is the parent (SIOCNT
/// reports multiplayer mode with no error) it arms timer 3 to clock the
/// transfers. Returns the link status (see SerialRuntime_ExchangePayloads).
s32 SerialRuntime_PollAndTransfer(void *send, void *receive) {
  volatile u32 *siocnt;
  u32 control;
  s32 status;
  s32 result;

  siocnt = &REG_SIOCNT;
  control = *siocnt;
  switch (gSerialRuntime.phase) {
  case 0: {
    u8 mask30 = control & 0x30;
    u8 mode;

    if (mask30 == 0 && (mode = control & 0x88) == 8) {
      u8 serialError = control & 4;

      if (serialError == 0 && gSerialRuntime.sendIndex == -1) {
        u32 ie;

        Io_Write16(0, &REG_IME);
        ie = REG_IE;
        Io_Write16((ie & ~INTR_FLAG_SERIAL) | INTR_FLAG_TIMER3, &REG_IE);
        Io_Write16(1, &REG_IME);
        *((volatile s8 *)siocnt + 1) &= ~0x40;
        REG_IF = INTR_FLAG_SERIAL | INTR_FLAG_TIMER3;
        REG_TM3CNT = 0xc963;
        gSerialRuntime.mode = mode;
      }
    } else if (mask30 == 0) {
      break;
    }
    gSerialRuntime.phase = 1;
  }
    // Fall through.
  case 1:
    SerialRuntime_CollectReceivedPayloads(receive);
    SerialRuntime_PrepareSendPacket(send);
    break;
  }

  gSerialRuntime.sequence++;
  status = gSerialRuntime.currentMask | (gSerialRuntime.receivedMask << 8);
  if (gSerialRuntime.mode == 8)
    status |= 0x80;
  result = SerialRuntime_AddParentFlag(status);
  if (((control << 26) >> 30) > 1)
    result |= 0x2000;
  return result;
}

/// As SerialRuntime_PollAndTransfer, without exchanging data: the frame loop
/// calls it, the VBlank handler does the exchange.
s32 SerialRuntime_PollStatus(void) {
  volatile u32 *siocnt;
  u32 control;
  s32 status;
  s32 result;

  siocnt = &REG_SIOCNT;
  control = *siocnt;
  if (gSerialRuntime.phase == 0) {
    u8 mode = control & 0x88;

    if (mode == 8) {
      u8 serialError = control & 4;

      if (serialError == 0 && gSerialRuntime.sendIndex == -1) {
        u32 ie;

        Io_Write16(0, &REG_IME);
        ie = REG_IE;
        Io_Write16((ie & ~INTR_FLAG_SERIAL) | INTR_FLAG_TIMER3, &REG_IE);
        Io_Write16(1, &REG_IME);
        *((volatile s8 *)siocnt + 1) &= ~0x40;
        REG_IF = INTR_FLAG_SERIAL | INTR_FLAG_TIMER3;
        REG_TM3CNT = 0xc963;
        gSerialRuntime.mode = mode;
      }
      gSerialRuntime.phase = 1;
    }
    gSerialRuntime.sequence++;
  }

  status = gSerialRuntime.currentMask | (gSerialRuntime.receivedMask << 8);
  if (gSerialRuntime.mode == 8)
    status |= 0x80;
  result = SerialRuntime_AddParentFlag(status);
  if (((control << 26) >> 30) > 1)
    result |= 0x2000;
  return result;
}

/// Once connected (phase 1), gathers the received payloads into `receive` and
/// builds the next packet from `send`. Returns the link status:
///   bits 0 to 7  : the consoles heard from this frame (bit 7 also when parent)
///   bits 8 to 15 : the consoles heard from since the link began
///   0x1000       : this console is the parent
///   0x2000       : SIOCNT's multiplayer id is 2 or more
s32 SerialRuntime_ExchangePayloads(void *send, void *receive) {
  volatile u32 *siocnt;
  u32 control;
  s32 status;
  s32 result;

  siocnt = &REG_SIOCNT;
  control = *siocnt;
  if (gSerialRuntime.phase == 1) {
    SerialRuntime_CollectReceivedPayloads(receive);
    SerialRuntime_PrepareSendPacket(send);
    gSerialRuntime.sequence++;
  }
  status = gSerialRuntime.currentMask | (gSerialRuntime.receivedMask << 8);
  if (gSerialRuntime.mode == 8)
    status |= 0x80;
  result = SerialRuntime_AddParentFlag(status);
  // SIOCNT bits 4 and 5, the multiplayer id
  if (((control << 26) >> 30) > 1)
    result |= 0x2000;
  return result;
}

/// Builds the packet to send in sendBuffer[0]: [0] the sequence number, [1]
/// the consoles not yet heard from, [2] the checksum and [4] the 24-byte
/// payload. The checksum is the complement of the sum of the first 28 bytes
/// as halfwords. Then restarts the send position and the timer.
void SerialRuntime_PrepareSendPacket(void *payload) {
  /* FAKEMATCH: volatile keeps the second load of sr->sendBuffer[0], as the
   * bytes do; through a plain pointer CSE reuses the first (differ 8, 48
   * instructions either way). */
  volatile struct SerialRuntime *sr;
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
    Dma_Set(payload, pkt + 4, (DMA_ENABLE | DMA_32BIT) << 16 | 6, REG_DMA3);
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
/// console). Returns the mask of consoles heard from this frame.
u8 SerialRuntime_CollectReceivedPayloads(void *payload) {
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
    swap = gSerialRuntime.pendingBuffer[i];
    gSerialRuntime.pendingBuffer[i] = gSerialRuntime.readyBuffer[i];
    gSerialRuntime.readyBuffer[i] = swap;
  }
  *(u32 *)flags = *(u32 *)gSerialRuntime.channelFlags;
  *(u32 *)gSerialRuntime.channelFlags = 0;
  Io_Write16(1, &REG_IME);
  gSerialRuntime.currentMask = 0;
  for (i = 0; i <= 1; i++) {
    packet = gSerialRuntime.pendingBuffer[i];
    sum = 0;
    for (k = 0; k < SERIAL_PACKET_HALFWORDS; k++)
      sum += packet[k];
    if (flags[i] == 1 && (s16)sum == -1) {
      Dma_Set(gSerialRuntime.pendingBuffer[i] + 2,
              (u8 *)payload + i * SERIAL_PAYLOAD_BYTES,
              (DMA_ENABLE | DMA_32BIT) << 16 | 6, REG_DMA3);
      gSerialRuntime.currentMask |= 1 << i;
    }
    if ((s16)sum == -1)
      gSerialRuntime.pendingBuffer[i][1] = ~gSerialRuntime.pendingBuffer[i][1];
  }
  gSerialRuntime.receivedMask |= gSerialRuntime.currentMask;
  return gSerialRuntime.currentMask;
}

/// The serial and timer 3 interrupt: each one moves a halfword. It sends the
/// next halfword of the outgoing packet (0xfefe between packets) and stores the
/// halfword received from each console; a console's fourteenth halfword
/// completes its packet.
void SerialRuntime_HandleTransferInterrupt(void) {
  // The receive channels share one backedge; a do/while for each adds 50 bytes
  // in tbs-en.
  struct SerialRuntime *sendState;
  struct SerialRuntime *receiveState;
  struct SerialRuntime *tailState;
  u16 recv[4];
  volatile u32 *sioControl;
  s32 channel;

  *(u64 *)recv = *(volatile u64 *)REG_SIODATA32;
  sioControl = &REG_SIOCNT;
  sendState = &gSerialRuntime;
  sendState->isParent = (*sioControl << 25) >> 31;

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
  receiveState = &gSerialRuntime;
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
  tailState = &gSerialRuntime;
  if (tailState->isParent != 0)
    tailState->channelFlags[channel] |= 2;
  if (tailState->receiveIndex[channel] <= SERIAL_PACKET_HALFWORDS)
    tailState->receiveIndex[channel]++;
  channel++;
  receiveState = tailState;
  if (channel <= 1)
    goto receive_loop;

  if (tailState->mode == 8) {
    REG_TM3CNT_H = 0;
    REG_SIOCNT16 |= SIO_START;
    REG_TM3CNT_H = TIMER_ENABLE | TIMER_INTR_ENABLE;
  }
}

/// Stops the per-frame exchange and removes the link interrupt handlers.
void SerialRuntime_RemoveIrqHandlers(void) {
  gSerialExchangeActive = 0;
  Runtime_SetIrqHandler(IRQ_SERIAL, 0, 0);
  Runtime_SetIrqHandler(IRQ_TIMER3, 0, 0);
}

/// Waits until every status bit in `mask` is set, then returns this console's
/// multiplayer id.
u32 SerialRuntime_WaitForStatusMask(s32 mask) {
  if ((mask & gLinkStatus) != mask) {
    do {
      WaitFrames(1);
    } while ((mask & gLinkStatus) != mask);
  }
  return (REG_SIOCNT << 0x1A) >> 0x1E;
}

/// Starts sending `size` bytes from `source` to the partner in 20-byte blocks.
/// Returns -1 when a send is already running, 0 otherwise.
s32 SerialRuntime_BeginSend(void *source, s32 size) {
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

/// Starts receiving the partner's blocks into `destination`. Returns -1 when
/// a receive is already running, 0 otherwise.
s32 SerialRuntime_BeginReceive(void *destination) {
  struct SerialTransferState *state = &gSerialTransfer;
  u32 savedInterruptMaster;

  if (gSerialReceiveDest != 0)
    return -1;
  IRQ_LOCK(savedInterruptMaster);
  state->status = 0x81;
  gSerialReceiveDest = (s32)destination;
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

SERIAL_DEFINE_WAIT(SerialRuntime_WaitForSend, gSerialSendSource)
SERIAL_DEFINE_WAIT(SerialRuntime_WaitForReceive, gSerialReceiveDest)

/// Waits until both the send and the receive have finished, giving up after
/// 600000 frames.
void SerialRuntime_WaitForTransfers(void) {
  u32 count;

  count = 0;
  while (gSerialSendSource != 0 || gSerialReceiveDest != 0) {
    WaitFrames(1);
    count++;
    if (count >= SERIAL_TRANSFER_TIMEOUT_FRAMES)
      return;
  }
}

/// Returns bit 0 while a send runs and bit 1 while a receive runs.
s32 SerialRuntime_GetActiveTransfers(void) {
  s32 flags;

  flags = 0;
  if (gSerialSendSource != 0) {
    flags = 1;
  }
  if (gSerialReceiveDest != 0) {
    flags |= 2;
  }
  return flags;
}

void BattleLink_ResetTransferState(void) { SerialRuntime_ResetTransferState(); }

struct OneCell {
  u16 v;
};

/// Runs each VBlank after the exchange: moves the bulk transfer on by one
/// 20-byte block each way. The receiver acknowledges each block's sequence
/// number; when the sender sees an acknowledgement behind its own count it
/// rewinds and sends the missed blocks again.
void SerialRuntime_StepBlockTransfer(void) {
  /* FAKEMATCH: gSerialSendSize and gSerialBlockSequence are volatile
   * (serial_runtime.h), so every access is its own load or store; as a plain
   * gSerialBlockSequence, 258 instructions differ in tbs-en. */
  struct SerialTransferState *peer;
  u32 id;
  struct SerialTransferState *own;
  u32 gap;
  /* FAKEMATCH: the halfword record is a HImode 1 loaded from the pool into r8
   * at entry; it is not protocol state. As the constant 1 at both stores, 55
   * of 257 instructions differ in tbs-en. */
  struct OneCell one;

  id = ~((REG_SIOCNT << 26) >> 30) & 1;
  one.v = 1;
  own = &gSerialTransfer;
  peer = (struct SerialTransferState *)gSerialPeerPayloads[id];
  if ((gLinkStatus & 3) != 3)
    return;
  // Receiving advances only when the peer presents the expected sequence.
  {
    s32 *receive = &gSerialReceiveDest;
    s32 dest = *receive;
    if (dest != 0) {
      if (own->flags == 1 && (u8)(peer->peerFlags - 1) <= 1) {
        if (peer->active == (gSerialBlockSequence & 0x7f)) {
          own->active = 0;
          switch (peer->peerFlags) {
          case 1:
            Dma_Set(peer->payload, (void *)dest,
                    (DMA_ENABLE | DMA_32BIT) << 16 | 5, REG_DMA3);
            *receive += SERIAL_BLOCK_BYTES;
            gSerialReceivedSize += SERIAL_BLOCK_BYTES;
            own->status = (own->status + 1) | 0x80;
            break;
          case 2:
            Dma_Set(peer->payload, (void *)dest,
                    (DMA_ENABLE | DMA_32BIT) << 16 | 5, REG_DMA3);
            gSerialReceivedSize += SERIAL_BLOCK_BYTES;
            own->flags = 2;
            own->status = 0;
            own->active = one.v;
            break;
          }
          gSerialBlockSequence = (gSerialBlockSequence + 1) & 0x7f;
        } else if (gSerialBlockSequence & 0x80) {
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
      if (peer->flags == 1) {
        if (peer->active & 0x80) {
          gap = (gSerialBlockSequence - peer->active) & 0x7f;
          *send = src - gap * SERIAL_BLOCK_BYTES;
          gSerialSendSize += gap * SERIAL_BLOCK_BYTES;
          gSerialBlockSequence -= gap;
          gSerialBlockSequence &= 0x7f;
        }
        if (gSerialSendSize != 0) {
          Dma_Set((void *)*send, own->payload,
                  (DMA_ENABLE | DMA_32BIT) << 16 | 5, REG_DMA3);
          gSerialSendSize += -SERIAL_BLOCK_BYTES;
          if (gSerialSendSize != 0) {
            own->peerFlags = 1;
            own->active = gSerialBlockSequence & 0x7f;
            *send += SERIAL_BLOCK_BYTES;
          } else {
            own->peerFlags = 2;
            own->active = gSerialBlockSequence & 0x7f;
            *send += SERIAL_BLOCK_BYTES;
          }
          gSerialBlockSequence = (gSerialBlockSequence + 1) & 0x7f;
        }
      }
      if (own->peerFlags == 2 && peer->flags == 2) {
        gSerialSendSource = 0;
        own->peerFlags = 0;
        own->active = 1;
      }
    }
  }
  if (own->flags == 2) {
    if (peer->peerFlags != 2) {
      gSerialReceiveDest = 0;
      own->flags = 0;
    }
  } else {
    own->flags = 0;
    if (gSerialReceiveDest != 0)
      own->flags = 1;
  }
}

extern const u8 Rom_Start[];

/// An empty routine the link test calls to waste time.
void SerialTest_Delay(void) {}

/// A link cable test left in the game: it shows a row of tiles, receives blocks
/// into the map cell buffer and shows them as tiles, and sends a part of the
/// ROM when A or B is held. It never returns.
void SerialTest_Run(void) {
  // An outer receive loop with an inner completion break changes 9 bytes in
  // tbs-en: the key-address load is hoisted and the restart retargeted.
  u16 *tile;
  s32 value;
  s32 n;
  s32 tick;

  AudioCommand_PlayFar(MUS_003);
  SerialRuntime_Initialize();
  n = 19;
  tile = (u16 *)BG_SCREEN_BLOCK(4) + 16 * 32 + 19;
  value = (s16)0xf093;
  do {
    n--;
    *tile = value;
    tile--;
    value--;
  } while (n >= 0);
  Cpu_Fill32(0, gMapCellBuffer, 0x400);
  SerialRuntime_WaitForStatusMask(3);
restart:
  SerialRuntime_BeginReceive(gMapCellBuffer);
  for (;;) {
    if (gKeysHeld & KEY_A)
      SerialRuntime_BeginSend((void *)Cart_Restart, 0x280);
    if (gKeysHeld & KEY_B)
      SerialRuntime_BeginSend((void *)(Rom_Start + 0x1000), 0x280);
    if (gKeysHeld & KEY_START) {
      tick = 9999;
      do {
        tick--;
        SerialTest_Delay();
      } while (tick >= 0);
    }
    if (gSerialReceiveDest == 0) {
      Dma_Set(gMapCellBuffer, (void *)0x06001000,
              (DMA_ENABLE | DMA_32BIT) << 16 | 0xa0, REG_DMA3);
      goto restart;
    }
    WaitFrames(1);
  }
}

#undef SERIAL_DEFINE_WAIT
