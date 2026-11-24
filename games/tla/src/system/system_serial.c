/// @file
/// The link cable: payload exchange, block transfers and the link test.
// Part of system.c.

#include "system_internal.h"
#include "sounds.h"
#include "field/actor.h"

enum {
  /// A packet: its number, missing mask, checksum and the 24-byte payload.
  SERIAL_PACKET_HALFWORDS = 14,
  SERIAL_PAYLOAD_BYTES = 24, ///< one console's payload each frame
  SERIAL_BLOCK_BYTES = 20,   ///< bulk data carried in each payload
  /// About 10,000 seconds, nearly three hours.
  SERIAL_TRANSFER_TIMEOUT_FRAMES = 600000,
  SERIAL_IDLE_HALFWORD = 0xfefe, ///< sent and seen between packets
  /// Timer 3's reload: it overflows 13981 cycles on, from one halfword
  /// transfer to the next, about 20 a frame.
  SERIAL_TIMER_RELOAD = 65536 - 13981
};

/// Sets the routine the save calls between sector writes (0 for none) and
/// starts its count again at 0 of SAVE_SECTORS_PER_PART; does nothing while
/// no save workspace (heap slot HEAP_SLOT_SAVE) is allocated.
void Save_SetProgressCallback(void (*progress)(s32 step)) {
  struct SaveWorkspace *state = Ram_HeapSlots->saveState;

  if (state == 0)
    return;
  state->progress = progress;
  state->sectorCount = 0;
  state->sectorTotal = SAVE_SECTORS_PER_PART;
}

/// Resets the serial port into multiplayer mode, installs the serial and timer
/// 3 interrupt handlers, clears gSerialState and points its packet buffers into
/// its storage, clears the bulk transfer state and sets gSerialExchangeActive,
/// which starts the per-frame exchange.
void Serial_Initialize(void) {
  u32 savedIme;
  s32 index;
  struct SerialState *state;

  IRQ_LOCK(savedIme);
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
  state = &gSerialState;
  REG_IME = 1;
  Dma_Fill32(0, state, sizeof(*state));
  state->sendIndex = -1;
  state->sendBuffer[0] = (u16 *)(state->storage + 0);
  state->sendBuffer[1] = (u16 *)(state->storage + SERIAL_PACKET_BYTES);
  // Each console has separate incoming, completed and pending packet buffers.
  for (index = 0; index <= 1; index++) {
    state->incomingBuffer[index] =
        (u16 *)(state->storage + SERIAL_PACKET_BYTES * 2 +
                index * SERIAL_PACKET_BYTES * 3);
    state->readyBuffer[index] =
        (u16 *)(state->storage + SERIAL_PACKET_BYTES * 3 +
                index * SERIAL_PACKET_BYTES * 3);
    state->pendingBuffer[index] =
        (u16 *)(state->storage + SERIAL_PACKET_BYTES * 4 +
                index * SERIAL_PACKET_BYTES * 3);
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
  IRQ_UNLOCK(savedIme);
}

/// Lets the timer start transfers again once a link mode is set, by setting
/// gSerialState.transferEnabled.
void Serial_EnableTransferTimer(void) {
  struct SerialState *state = &gSerialState;

  if (state->mode != 0)
    state->transferEnabled = 1;
}

/// Disables the serial and timer 3 interrupts, stops the transfer timer and
/// clears gSerialState.transferEnabled. Nothing in the game calls it.
void Serial_DisableTransfer(void) {
  struct SerialState *state;

  REG_IME = 0;
  REG_IE &= (u16)~(INTR_FLAG_SERIAL | INTR_FLAG_TIMER3);
  REG_IME = 1;
  REG_SIOCNT16 = SIO_MULTI_MODE | SIO_115200_BPS;
  // Timer 3 stopped, with its reload set
  REG_TM3CNT = SERIAL_TIMER_RELOAD;
  REG_IF = INTR_FLAG_SERIAL | INTR_FLAG_TIMER3;
  state = &gSerialState;
  state->transferEnabled = 0;
}

/// Looks for the link partner and, once connected, does one exchange (as
/// Serial_ExchangePayloads).
s32 Serial_PollAndTransfer(void *send, void *receive) {
  volatile u32 *siocnt;
  u32 control;
  s32 status;
  s32 result;

  siocnt = &REG_SIOCNT;
  control = *siocnt;
  switch (gSerialState.phase) {
  case SERIAL_PHASE_SEARCHING: {
    u8 multiplayerId = control & SIO_ID;
    u8 mode;

    // The parent waits here until SIO_MULTI_SD is set with SIO_START clear;
    // a child connects at once.
    if (multiplayerId == 0 &&
        (mode = control & (SIO_START | SIO_MULTI_SD)) == SERIAL_MODE_CLOCKING) {
      u8 serialError = control & SIO_MULTI_SI;

      if (serialError == 0 && gSerialState.sendIndex == -1) {

        Io_Write16(0, &REG_IME);
        {
          u32 ie = REG_IE;

          Io_Write16((ie & ~INTR_FLAG_SERIAL) | INTR_FLAG_TIMER3, &REG_IE);
          Io_Write16(1, &REG_IME);
          // SIOCNT's high byte: clear SIO_INTR_ENABLE, as timer 3 now clocks
          // the transfers
          *((volatile s8 *)siocnt + 1) &= ~(SIO_INTR_ENABLE >> 8);
          REG_IF = INTR_FLAG_SERIAL | INTR_FLAG_TIMER3;
          REG_TM3CNT = SERIAL_TIMER_RELOAD;
          gSerialState.mode = mode;
        }
      }
    } else if (multiplayerId == 0) {
      break;
    }
    gSerialState.phase = SERIAL_PHASE_CONNECTED;
  }
    // Fall through.
  case SERIAL_PHASE_CONNECTED:
    Serial_CollectReceivedPayloads(receive);
    Serial_PrepareSendPacket(send);
    break;
  }

  gSerialState.packetNumber++;
  status = gSerialState.currentMask | (gSerialState.receivedMask << 8);
  if (gSerialState.mode == SERIAL_MODE_CLOCKING)
    status |= LINK_STATUS_CLOCKING;
  result = Serial_AddErrorFlag(status);
  if (SIO_ID_OF(control) > 1)
    result |= LINK_STATUS_EXTRA_CONSOLE;
  return result;
}

/// As Serial_PollAndTransfer, without exchanging data: wait() calls
/// it once a frame while gSerialExchangeActive is set, and the VBlank handler
/// does the exchange.
s32 Serial_PollStatus(void) {
  volatile u32 *siocnt;
  u32 control;
  s32 status;
  s32 result;

  siocnt = &REG_SIOCNT;
  control = *siocnt;
  if (gSerialState.phase == SERIAL_PHASE_SEARCHING) {
    u8 mode = control & (SIO_START | SIO_MULTI_SD);

    if (mode == SERIAL_MODE_CLOCKING) {
      u8 serialError = control & SIO_MULTI_SI;

      if (serialError == 0 && gSerialState.sendIndex == -1) {

        Io_Write16(0, &REG_IME);
        {
          u32 ie = REG_IE;

          Io_Write16((ie & ~INTR_FLAG_SERIAL) | INTR_FLAG_TIMER3, &REG_IE);
          Io_Write16(1, &REG_IME);
          // SIOCNT's high byte: clear SIO_INTR_ENABLE, as timer 3 now clocks
          // the transfers
          *((volatile s8 *)siocnt + 1) &= ~(SIO_INTR_ENABLE >> 8);
          REG_IF = INTR_FLAG_SERIAL | INTR_FLAG_TIMER3;
          REG_TM3CNT = SERIAL_TIMER_RELOAD;
          gSerialState.mode = mode;
        }
      }
      gSerialState.phase = SERIAL_PHASE_CONNECTED;
    }
    gSerialState.packetNumber++;
  }

  status = gSerialState.currentMask | (gSerialState.receivedMask << 8);
  if (gSerialState.mode == SERIAL_MODE_CLOCKING)
    status |= LINK_STATUS_CLOCKING;
  result = Serial_AddErrorFlag(status);
  if (SIO_ID_OF(control) > 1)
    result |= LINK_STATUS_EXTRA_CONSOLE;
  return result;
}

/// Once connected (SERIAL_PHASE_CONNECTED), gathers the received payloads into
/// `receive` and builds the next packet from `send`. Called by the VBlank
/// handler while gSerialExchangeActive is set, which stores the result in
/// gLinkStatus. Returns the link status: in bits 0 and 1 the consoles heard
/// from this frame (both: LINK_STATUS_HEARD_BOTH, which the block transfer and
/// Serial_WaitForStatusMask wait for), in bits 8 and 9 those heard from since
/// the link began, and LINK_STATUS_CLOCKING, LINK_STATUS_ERROR and
/// LINK_STATUS_EXTRA_CONSOLE.
s32 Serial_ExchangePayloads(void *send, void *receive) {
  volatile u32 *siocnt;
  u32 control;
  s32 status;
  s32 result;

  siocnt = &REG_SIOCNT;
  control = *siocnt;
  if (gSerialState.phase == SERIAL_PHASE_CONNECTED) {
    Serial_CollectReceivedPayloads(receive);
    Serial_PrepareSendPacket(send);
    gSerialState.packetNumber++;
  }
  status = gSerialState.currentMask | (gSerialState.receivedMask << 8);
  if (gSerialState.mode == SERIAL_MODE_CLOCKING)
    status |= LINK_STATUS_CLOCKING;
  result = Serial_AddErrorFlag(status);
  // SIOCNT bits 4 and 5, the multiplayer id
  if (SIO_ID_OF(control) > 1)
    result |= LINK_STATUS_EXTRA_CONSOLE;
  return result;
}

/// Builds the packet to send in sendBuffer[0] (struct SerialPacket): the packet
/// number, the consoles not yet heard from, the checksum and the 24-byte
/// payload.
void Serial_PrepareSendPacket(void *payload) {
  volatile struct SerialState *state;
  u32 i;
  u32 sum;

  state = SERIAL_RUNTIME;
  {
    // volatile: the serial interrupt sends from this buffer. The bytes of
    // struct SerialPacket, written through a byte pointer: as the struct's
    // volatile members, each store would read its word first.
    volatile u8 *packet;
    u8 currentMask;
    u8 receivedMask;

    packet = (volatile u8 *)state->sendBuffer[0];
    packet[offsetof(struct SerialPacket, packetNumber)] = state->packetNumber;
    currentMask = state->currentMask;
    receivedMask = state->receivedMask;
    sum = 0;
    *(volatile u16 *)(packet + offsetof(struct SerialPacket, checksum)) = sum;
    packet[offsetof(struct SerialPacket, missingMask)] =
        receivedMask ^ currentMask;
    Dma_Set(payload, (void *)(packet + offsetof(struct SerialPacket, payload)),
            DMA_COPY_WORDS(SERIAL_PAYLOAD_BYTES / 4), REG_DMA3);
  }
  {
    u16 *halfword;
    u32 value;

    halfword = state->sendBuffer[0];
    for (i = 0; i < SERIAL_PACKET_HALFWORDS; i++) {
      value = *halfword;
      halfword++;
      sum += value;
    }
  }
  {
    u32 checksum;
    struct SerialPacket *packet;

    checksum = ~sum;
    packet = (struct SerialPacket *)state->sendBuffer[0];
    packet->checksum = checksum;
  }

  // Stop timer 3, rewind the send position and start the timer again
  if (state->mode != 0)
    REG_TM3CNT_H = 0;
  state->sendIndex = -1;
  if (state->mode != 0 && state->transferEnabled != 0)
    REG_TM3CNT_H = TIMER_INTR_ENABLE | TIMER_ENABLE;
}

/// Swaps in the packets the interrupt handler finished, then copies the payload
/// of each one whose checksum is good to `payload` (24 bytes per console), and
/// records the consoles heard from in currentMask and receivedMask.
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
    swap = gSerialState.pendingBuffer[i];
    gSerialState.pendingBuffer[i] = gSerialState.readyBuffer[i];
    gSerialState.readyBuffer[i] = swap;
  }
  *(u32 *)flags = *(u32 *)gSerialState.channelFlags;
  *(u32 *)gSerialState.channelFlags = 0;
  Io_Write16(1, &REG_IME);
  gSerialState.currentMask = 0;
  for (i = 0; i <= 1; i++) {
    packet = gSerialState.pendingBuffer[i];
    sum = 0;
    for (k = 0; k < SERIAL_PACKET_HALFWORDS; k++)
      sum += packet[k];
    // A good packet's halfwords, its complemented checksum included, sum to
    // 0xffff.
    if (flags[i] == SERIAL_CHANNEL_PACKET_COMPLETE && (s16)sum == -1) {
      Dma_Set(gSerialState.pendingBuffer[i] + 2,
              (u8 *)payload + i * SERIAL_PAYLOAD_BYTES,
              DMA_COPY_WORDS(SERIAL_PAYLOAD_BYTES / 4), REG_DMA3);
      gSerialState.currentMask |= 1 << i;
    }
    // Complementing a halfword spoils the sum, so a buffer that comes round
    // again unchanged is never taken twice
    if ((s16)sum == -1)
      gSerialState.pendingBuffer[i][1] = ~gSerialState.pendingBuffer[i][1];
  }
  gSerialState.receivedMask |= gSerialState.currentMask;
  return gSerialState.currentMask;
}

/// The serial and timer 3 interrupt, installed by Serial_Initialize: each one
/// moves a halfword.
void Serial_HandleTransferInterrupt(void) {
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
  // SIOCNT's error bit (SIO_ERROR): a packet heard while it is set is refused
  // (SERIAL_CHANNEL_ERROR, below)
  gSerialState.transferError = SIO_ERROR_OF(*sioControl);

  if (gSerialState.sendIndex == -1) {
    u32 idleWord = SERIAL_IDLE_HALFWORD;
    u16 *swap;

    ((volatile u16 *)sioControl)[1] = idleWord;
    swap = gSerialState.sendBuffer[1];
    gSerialState.sendBuffer[1] = gSerialState.sendBuffer[0];
    gSerialState.sendBuffer[0] = swap;
  } else if (gSerialState.sendIndex >= 0) {
    ((volatile u16 *)sioControl)[1] =
        gSerialState.sendBuffer[1][gSerialState.sendIndex];
  }
  if (gSerialState.sendIndex <= SERIAL_PACKET_HALFWORDS)
    gSerialState.sendIndex++;

  for (channel = 0; channel <= 1; channel++) {
    s32 receiveIndex;
    u16 *incoming;

    // Hand complete packets to the frame code without overwriting its buffers.
    if (recv[channel] == SERIAL_IDLE_HALFWORD &&
        gSerialState.receiveIndex[channel] >= SERIAL_PACKET_HALFWORDS) {
      gSerialState.receiveIndex[channel] = -1;
    } else {
      receiveIndex = gSerialState.receiveIndex[channel];
      incoming = gSerialState.incomingBuffer[channel];
      incoming[receiveIndex] = recv[channel];
      if (receiveIndex == SERIAL_PACKET_HALFWORDS - 1) {
        u16 *swap = gSerialState.readyBuffer[channel];

        gSerialState.readyBuffer[channel] = incoming;
        gSerialState.incomingBuffer[channel] = swap;
        gSerialState.channelFlags[channel] |= SERIAL_CHANNEL_PACKET_COMPLETE;
      }
    }
    if (gSerialState.transferError != 0)
      gSerialState.channelFlags[channel] |= SERIAL_CHANNEL_ERROR;
    if (gSerialState.receiveIndex[channel] <= SERIAL_PACKET_HALFWORDS)
      gSerialState.receiveIndex[channel]++;
  }

  // The console that clocks the transfers starts the next one, and restarts
  // timer 3 so its count reloads
  if (gSerialState.mode == SERIAL_MODE_CLOCKING) {
    REG_TM3CNT_H = 0;
    REG_SIOCNT16 |= SIO_START;
    REG_TM3CNT_H = TIMER_ENABLE | TIMER_INTR_ENABLE;
  }
}

/// Clears gSerialExchangeActive, which stops the per-frame exchange, and
/// removes the link interrupt handlers.
void Serial_RemoveIrqHandlers(void) {
  gSerialExchangeActive = 0;
  Irq_SetHandler(IRQ_SERIAL, 0, 0);
  Irq_SetHandler(IRQ_TIMER3, 0, 0);
}

/// Waits a frame at a time until every status bit in `mask` is set in
/// gLinkStatus, then returns this console's multiplayer id.
u32 Serial_WaitForStatusMask(s32 mask) {
  while ((mask & gLinkStatus) != mask) {
    wait(1);
  }
  return SIO_ID_OF(REG_SIOCNT);
}

/// Starts sending `size` bytes from `source` to the partner in blocks of
/// SERIAL_BLOCK_BYTES: sets the send source, size and block number that
/// Serial_StepBlockTransfer works from.
s32 Serial_StartSend(void *source, s32 size) {
  struct SerialTransferState *state = &gSerialTransfer;
  u32 savedIme;

  if (gSerialSendSource != 0)
    return -1;
  IRQ_LOCK(savedIme);
  state->status = SERIAL_STATUS_RUNNING;
  gSerialSendSource = (s32)source;
  gSerialSendSize = size;
  gSerialBlockNumber = 0;
  state->active = 1;
  IRQ_UNLOCK(savedIme);
  return 0;
}

/// Starts receiving the partner's blocks into `destination`: sets the receive
/// destination and clears the received size.
s32 Serial_StartReceive(void *destination) {
  struct SerialTransferState *state = &gSerialTransfer;
  u32 savedIme;

  if (gSerialReceiveDestination != 0)
    return -1;
  IRQ_LOCK(savedIme);
  state->status = SERIAL_STATUS_RUNNING | 1;
  gSerialReceiveDestination = (s32)destination;
  gSerialReceivedSize = 0;
  state->active = 1;
  gSerialBlockNumber = 0;
  IRQ_UNLOCK(savedIme);
  return 0;
}

/// Waits a frame at a time until the queued send is done, giving up after
/// SERIAL_TRANSFER_TIMEOUT_FRAMES frames.
void Serial_WaitForSend(void) {
  u32 count = 0;

  if (gSerialSendSource != 0) {
    for (; count < SERIAL_TRANSFER_TIMEOUT_FRAMES && gSerialSendSource != 0;
         count++) {
      wait(1);
    }
  }
}

/// Waits a frame at a time until the queued receive is done, giving up after
/// SERIAL_TRANSFER_TIMEOUT_FRAMES frames.
void Serial_WaitForReceive(void) {
  u32 count = 0;

  if (gSerialReceiveDestination != 0) {
    do {
      wait(1);
      count++;
    } while (count < SERIAL_TRANSFER_TIMEOUT_FRAMES &&
             gSerialReceiveDestination != 0);
  }
}

/// Waits until both the send and the receive have finished, giving up after
/// SERIAL_TRANSFER_TIMEOUT_FRAMES frames. Reached through the far-call table.
void Serial_WaitForTransfers(void) {
  u32 count;

  count = 0;
  while (gSerialSendSource != 0 || gSerialReceiveDestination != 0) {
    wait(1);
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

/// Clears the serial transfer for a link battle with interrupts off: nothing to
/// send or receive, no flags, and status SERIAL_STATUS_RUNNING.
void LinkBattle_ResetTransferState(void) {
  struct SerialTransferState *state;
  u32 saved;

  state = &gSerialTransfer;
  IRQ_LOCK(saved);
  state->status = SERIAL_STATUS_RUNNING;
  *(u8 *)&state->peerFlags = 0;
  gSerialSendSource = 0;
  *(u8 *)&state->flags = 0;
  gSerialSendSize = 0;
  gSerialReceiveDestination = 0;
  gSerialReceivedSize = 0;
  IRQ_UNLOCK(saved);
}

/// Runs each VBlank after the exchange, called by System_VBlankHandler: moves
/// the bulk transfer on by one SERIAL_BLOCK_BYTES block each way.
void Serial_StepBlockTransfer(void) {
  struct SerialTransferState *peer;
  u32 id;
  struct SerialTransferState *own;
  u32 gap;
  /* FAKEMATCH(temporary): the 1 kept in a halfword record. */
  struct SerialHalfword one;

  // The partner's payload: of multiplayer ids 0 and 1, the one not ours.
  id = ~(SIO_ID_OF(REG_SIOCNT)) & 1;
  one.value = 1;
  own = &gSerialTransfer;
  peer = &gSerialPeerPayloads[id];
  // Both consoles must have been heard from this frame
  if ((gLinkStatus & LINK_STATUS_HEARD_BOTH) != LINK_STATUS_HEARD_BOTH)
    return;
  // Receiving advances only when the peer presents the expected block number.
  // peerFlags is the sender's block kind, an enum SerialSendState.
  if (gSerialReceiveDestination != 0) {
    if (own->flags == SERIAL_RECEIVE_ACTIVE &&
        (u8)(peer->peerFlags - SERIAL_SEND_MORE) <=
            SERIAL_SEND_LAST - SERIAL_SEND_MORE) {
      if (peer->active == (gSerialBlockNumber & SERIAL_BLOCK_NUMBER_MASK)) {
        own->active = 0;
        switch (peer->peerFlags) {
        case SERIAL_SEND_MORE:
          // A block with more to come: store it and move on
          Dma_Set(peer->payload, (void *)gSerialReceiveDestination,
                  DMA_COPY_WORDS(SERIAL_BLOCK_BYTES / 4), REG_DMA3);
          gSerialReceiveDestination += SERIAL_BLOCK_BYTES;
          gSerialReceivedSize += SERIAL_BLOCK_BYTES;
          own->status = (own->status + 1) | SERIAL_STATUS_RUNNING;
          break;
        case SERIAL_SEND_LAST:
          // The last block: store it and report the receive finished
          Dma_Set(peer->payload, (void *)gSerialReceiveDestination,
                  DMA_COPY_WORDS(SERIAL_BLOCK_BYTES / 4), REG_DMA3);
          gSerialReceivedSize += SERIAL_BLOCK_BYTES;
          own->flags = SERIAL_RECEIVE_DONE;
          own->status = 0;
          own->active = one.value;
          break;
        }
        gSerialBlockNumber =
            (gSerialBlockNumber + 1) & SERIAL_BLOCK_NUMBER_MASK;
      } else if (gSerialBlockNumber & SERIAL_BLOCK_REWIND) {
        // Out of step and already asked: bit 7 of active is the request to
        // rewind to the block number in its low bits
        if (own->active & SERIAL_BLOCK_REWIND) {
          own->active = one.value;
        } else if (own->active == 1) {
          own->active = 0;
          gSerialBlockNumber &= SERIAL_BLOCK_NUMBER_MASK;
        }
      } else {
        own->active = gSerialBlockNumber | SERIAL_BLOCK_REWIND;
        gSerialBlockNumber |= SERIAL_BLOCK_REWIND;
      }
    } else {
      own->active = 0;
    }
  }
  // A peer behind our block number requests a rewind before the next block.
  if (gSerialSendSource != 0) {
    if (peer->flags == SERIAL_RECEIVE_ACTIVE) {
      if (peer->active & SERIAL_BLOCK_REWIND) {
        gap = (gSerialBlockNumber - peer->active) & SERIAL_BLOCK_NUMBER_MASK;
        gSerialSendSource -= gap * SERIAL_BLOCK_BYTES;
        gSerialSendSize += gap * SERIAL_BLOCK_BYTES;
        gSerialBlockNumber -= gap;
        gSerialBlockNumber &= SERIAL_BLOCK_NUMBER_MASK;
      }
      // Send the next block, marked as the last when it empties the size
      if (gSerialSendSize != 0) {
        Dma_Set((void *)gSerialSendSource, own->payload,
                DMA_COPY_WORDS(SERIAL_BLOCK_BYTES / 4), REG_DMA3);
        gSerialSendSize -= SERIAL_BLOCK_BYTES;
        if (gSerialSendSize != 0) {
          own->peerFlags = SERIAL_SEND_MORE;
          own->active = gSerialBlockNumber & SERIAL_BLOCK_NUMBER_MASK;
          gSerialSendSource += SERIAL_BLOCK_BYTES;
        } else {
          own->peerFlags = SERIAL_SEND_LAST;
          own->active = gSerialBlockNumber & SERIAL_BLOCK_NUMBER_MASK;
          gSerialSendSource += SERIAL_BLOCK_BYTES;
        }
        gSerialBlockNumber =
            (gSerialBlockNumber + 1) & SERIAL_BLOCK_NUMBER_MASK;
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

extern u8 gMapGridCellBuffer[];

/// An empty routine the link test calls to waste time.
void SerialTest_Delay(void) {}

/// The link test's block: LINK_TEST_TILES tiles' worth of bytes, shown from
/// tile LINK_TEST_TILE of character block 0.
#define LINK_TEST_BYTES 640
#define LINK_TEST_TILES (LINK_TEST_BYTES / TILE_SIZE_4BPP)
#define LINK_TEST_TILE (2 * BG_SCREEN_SIZE / TILE_SIZE_4BPP)
/// What B sends: the cartridge's bytes from 4 KB in, where A sends its first.
#define LINK_TEST_SECOND_SOURCE (Rom_Start + 4096)

/// A link cable test left in the game: it shows a row of tiles, receives blocks
/// into the map grid cell buffer and shows them as tiles, and sends a part of
/// the ROM when A or B is held.
void SerialTest_Run(void) {
  u16 *tile;
  s32 value;
  s32 remaining;
  s32 delay;

  Sound_PlayFar(MUS_CHAMPA);
  Serial_Initialize();
  // Row 16, columns 0 to 19: the received tiles in WINDOW_PALETTE.
  remaining = LINK_TEST_TILES - 1;
  tile = (u16 *)BG_SCREEN_BLOCK(DEBUG_TEXT_SCREEN_BLOCK) +
         16 * BG_SCREEN_WIDTH + LINK_TEST_TILES - 1;
  value = (s16)(BG_TILE_PALETTE(WINDOW_PALETTE) |
                (LINK_TEST_TILE + LINK_TEST_TILES - 1));
  do {
    remaining--;
    *tile = value;
    tile--;
    value--;
  } while (remaining >= 0);
  Cpu_Fill32(0, gMapGridCellBuffer, 1024);
  Serial_WaitForStatusMask(LINK_STATUS_HEARD_BOTH);
restart:
  Serial_StartReceive(gMapGridCellBuffer);
  for (;;) {
    if (gInput.held & KEY_A)
      Serial_StartSend((void *)Cart_Start, LINK_TEST_BYTES);
    if (gInput.held & KEY_B)
      Serial_StartSend((void *)LINK_TEST_SECOND_SOURCE, LINK_TEST_BYTES);
    if (gInput.held & KEY_START) {
      for (delay = 0; delay < 10000; delay++)
        SerialTest_Delay();
    }
    if (gSerialReceiveDestination == 0) {
      Dma_Set(gMapGridCellBuffer,
              BG_CHAR_BLOCK(0) + LINK_TEST_TILE * TILE_SIZE_4BPP,
              DMA_COPY_WORDS(LINK_TEST_BYTES / 4), REG_DMA3);
      goto restart;
    }
    wait(1);
  }
}

#undef LINK_TEST_BYTES
#undef LINK_TEST_TILES
#undef LINK_TEST_TILE
#undef LINK_TEST_SECOND_SOURCE
