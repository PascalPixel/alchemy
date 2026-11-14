/// @file
/// The link cable: payload exchange, block transfers and the link test.
// Part of system.c.

#include "../system_internal.h"

enum {
  /// A packet: its number, missing mask, checksum and the 24-byte payload.
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
#include "sounds.h"
#include "actor.h"

void Serial_QueueCommand(u32 command) {
  struct SerialCommandWork *work = Ram_HeapSlots->serialWork;

  if (work == 0)
    return;
  work->command = command;
  work->progress = 0;
  work->state = 3;
}

void Serial_Initialize(void) {
  u32 interruptEnable;
  volatile u16 *ime;
  s32 index;
  struct SerialWork *state;

  // IRQ_LOCK written out, with the IME address set inside the save, as the
  // Broken Seal writes it.
  IME_SAVE((ime = &REG_IME), interruptEnable);
  IME_DISABLE(ime);
  Irq_SetHandler(7, 0, Serial_HandleTransferInterrupt);
  Irq_SetHandler(6, 0, Serial_HandleTransferInterrupt);
  REG_IME = 0;
  REG_IE &= 0xff3f;
  if ((REG_IF & 0x80) != 0)
    REG_IF = 0x80;
  if ((REG_IF & 0x40) != 0)
    REG_IF = 0x40;
  REG_RCNT = 0x8000;
  REG_RCNT = 0;
  REG_SIOCNT = 0x1000;
  REG_RCNT = 0;
  REG_SIOCNT = 0x2000;
  REG_SIOCNT16 = REG_SIOCNT16 | 0x4003;
  state = &gSerialWork;
  REG_IME = 1;
  Dma_Fill32(0, state, sizeof(*state));
  state->sendIndex = -1;
  state->sendBuffer[0] = (u16 *)(state->storage + 0);
  state->sendBuffer[1] = (u16 *)(state->storage + 32);
  for (index = 0; index <= 1; index++) {
    state->incomingBuffer[index] = (u16 *)(state->storage + 64 + index * 96);
    state->readyBuffer[index] = (u16 *)(state->storage + 96 + index * 96);
    state->pendingBuffer[index] = (u16 *)(state->storage + 128 + index * 96);
  }
  REG_IME = 0;
  REG_IE |= 0x80;
  REG_IME = 1;
  gSerialExchangeActive = 1;
  gLinkExchangeState = 0;
  gSerialSendSource = 0;
  gSerialSendSize = 0;
  gSerialReceiveDest = 0;
  gSerialReceivedSize = 0;
  LinkBattle_ResetTransferState();
  IRQ_UNLOCK(interruptEnable);
}

/// Lets the timer start transfers again once a link mode is set, by setting
/// gSerialWork.transferEnabled.
void Serial_EnableTransferTimer(void) {
  struct SerialWork *state = &gSerialWork;

  if (state->mode != 0)
    state->transferEnabled = 1;
}

/// Disables the serial and timer 3 interrupts, stops the transfer timer and
/// clears gSerialWork.transferEnabled. Nothing in the game calls it.
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
/// Serial_ExchangePayloads).
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
          gSerialWork.mode = mode;
        }
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

  gSerialWork.packetNumber++;
  status = gSerialWork.currentMask | (gSerialWork.receivedMask << 8);
  if (gSerialWork.mode == SERIAL_MODE_CLOCKING)
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
  if (gSerialWork.phase == SERIAL_PHASE_SEARCHING) {
    u8 mode = control & (SIO_START | SIO_MULTI_SD);

    if (mode == SERIAL_MODE_CLOCKING) {
      u8 serialError = control & SIO_MULTI_SI;

      if (serialError == 0 && gSerialWork.sendIndex == -1) {

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
          gSerialWork.mode = mode;
        }
      }
      gSerialWork.phase = SERIAL_PHASE_CONNECTED;
    }
    gSerialWork.packetNumber++;
  }

  status = gSerialWork.currentMask | (gSerialWork.receivedMask << 8);
  if (gSerialWork.mode == SERIAL_MODE_CLOCKING)
    status |= LINK_STATUS_CLOCKING;
  result = Serial_AddErrorFlag(status);
  if (SIO_ID_OF(control) > 1)
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
    gSerialWork.packetNumber++;
  }
  status = gSerialWork.currentMask | (gSerialWork.receivedMask << 8);
  if (gSerialWork.mode == SERIAL_MODE_CLOCKING)
    status |= LINK_STATUS_CLOCKING;
  result = Serial_AddErrorFlag(status);
  // SIOCNT bits 4 and 5, the multiplayer id
  if (SIO_ID_OF(control) > 1)
    result |= LINK_STATUS_EXTRA_CONSOLE;
  return result;
}

/// Builds the outgoing packet in sendBuffer[0], in the same format as above.
void Serial_PrepareSendPacket(void *payload) {
  volatile struct SerialWork *sr;
  u32 i;
  u32 sum;

  sr = SERIAL_RUNTIME;
  {
    // volatile: the serial interrupt sends from this buffer.
    volatile u8 *pkt;
    u8 cur;
    u8 recv;

    pkt = (volatile u8 *)sr->sendBuffer[0];
    pkt[0] = sr->packetNumber;
    cur = sr->currentMask;
    recv = sr->receivedMask;
    sum = 0;
    *(volatile u16 *)(pkt + 2) = sum;
    pkt[1] = recv ^ cur;
    Dma_Set(payload, (void *)(pkt + 4), DMA_COPY_WORDS(6), REG_DMA3);
  }
  {
    u16 *pkt;
    u32 v;

    pkt = sr->sendBuffer[0];
    for (i = 0; i <= 13; i++) {
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
  if (sr->mode != 0)
    REG_TM3CNT_H = 0;
  sr->sendIndex = -1;
  if (sr->mode != 0 && sr->transferEnabled != 0)
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
    // 0xffff.
    if (flags[i] == SERIAL_CHANNEL_PACKET_COMPLETE && (s16)sum == -1) {
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
  gSerialWork.transferError = (*sioControl << 25) >> 31;

  if (gSerialWork.sendIndex == -1) {
    u32 idleWord = SERIAL_IDLE_HALFWORD;
    u16 *swap;

    ((volatile u16 *)sioControl)[1] = idleWord;
    swap = gSerialWork.sendBuffer[1];
    gSerialWork.sendBuffer[1] = gSerialWork.sendBuffer[0];
    gSerialWork.sendBuffer[0] = swap;
  } else if (gSerialWork.sendIndex >= 0) {
    ((volatile u16 *)sioControl)[1] =
        gSerialWork.sendBuffer[1][gSerialWork.sendIndex];
  }
  if (gSerialWork.sendIndex <= SERIAL_PACKET_HALFWORDS)
    gSerialWork.sendIndex++;

  channel = 0;
receive_loop:
  // Hand complete packets to the frame code without overwriting its buffers.
  {
    s32 receiveIndex;
    u16 *incoming;

    if (recv[channel] == SERIAL_IDLE_HALFWORD &&
        gSerialWork.receiveIndex[channel] >= SERIAL_PACKET_HALFWORDS) {
      gSerialWork.receiveIndex[channel] = -1;
    } else {
      receiveIndex = gSerialWork.receiveIndex[channel];
      incoming = gSerialWork.incomingBuffer[channel];
      incoming[receiveIndex] = recv[channel];
      if (receiveIndex == SERIAL_PACKET_HALFWORDS - 1) {
        u16 *swap = gSerialWork.readyBuffer[channel];

        gSerialWork.readyBuffer[channel] = incoming;
        gSerialWork.incomingBuffer[channel] = swap;
        gSerialWork.channelFlags[channel] |= SERIAL_CHANNEL_PACKET_COMPLETE;
      }
    }
  }
  if (gSerialWork.transferError != 0)
    gSerialWork.channelFlags[channel] |= SERIAL_CHANNEL_ERROR;
  if (gSerialWork.receiveIndex[channel] <= SERIAL_PACKET_HALFWORDS)
    gSerialWork.receiveIndex[channel]++;
  channel++;
  if (channel <= 1)
    goto receive_loop;

  // The console that clocks the transfers starts the next one, and restarts
  // timer 3 so its count reloads
  if (gSerialWork.mode == SERIAL_MODE_CLOCKING) {
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

/// Starts sending `size` bytes from `source` to the partner in 20-byte blocks:
/// sets the send source, size and block number that Serial_StepBlockTransfer
/// works from.
s32 Serial_StartSend(void *source, s32 size) {
  struct SerialTransferState *state = &gSerialTransfer;
  u32 savedInterruptMaster;

  if (gSerialSendSource != 0)
    return -1;
  IRQ_LOCK(savedInterruptMaster);
  state->status = SERIAL_STATUS_RUNNING;
  gSerialSendSource = (s32)source;
  gSerialSendSize = size;
  gSerialBlockNumber = 0;
  state->active = 1;
  IRQ_UNLOCK(savedInterruptMaster);
  return 0;
}

s32 Serial_StartReceive(void *destination) {
  struct SerialTransferState *state = &gSerialTransfer;
  u32 saved;

  if (gSerialReceiveDest != 0)
    return -1;
  IRQ_LOCK(saved);
  state->status = 0x81;
  gSerialReceiveDest = (s32)destination;
  gSerialReceivedSize = 0;
  state->active = 1;
  gSerialBlockNumber = 0;
  IRQ_UNLOCK(saved);
  return 0;
}

void Serial_WaitForSend(void) {
  u32 count = 0;

  if (gSerialSendSource != 0) {
    do {
      wait(1);
      count++;
    } while (count <= 0x927bf && gSerialSendSource != 0);
  }
}

void Serial_WaitForReceive(void) {
  u32 count = 0;

  if (gSerialReceiveDest != 0) {
    do {
      wait(1);
      count++;
    } while (count <= 0x927bf && gSerialReceiveDest != 0);
  }
}

void Serial_WaitForTransfers(void) {
  u32 count = 0;

  while (gSerialSendSource != 0 || gSerialReceiveDest != 0) {
    wait(1);
    count++;
    if (count >= 600000)
      return;
  }
}

s32 Serial_GetActiveTransfers(void) {
  s32 flags;

  flags = 0;
  if (gSerialSendSource != 0)
    flags = 1;
  if (gSerialReceiveDest != 0)
    flags |= 2;
  return flags;
}

void LinkBattle_ResetTransferState(void) {
  struct SerialTransferState *state;
  volatile u16 *ime;
  u32 saved;
  u32 zero;

  state = &gSerialTransfer;
  ime = &REG_IME;
  /* FAKEMATCH(dead): the IME exchange in a once-only block. */
  do {
    saved = *ime;
    *ime = (u16)ime;
  } while (0);
  zero = 0;
  state->status = 0x80;
  *(u8 *)&state->peerFlags = zero;
  gSerialSendSource = zero;
  *(u8 *)&state->flags = zero;
  gSerialSendSize = zero;
  gSerialReceiveDest = zero;
  gSerialReceivedSize = zero;
  *ime = saved;
}

void Serial_StepBlockTransfer(void) {
  struct SerialTransferState *peer;
  u32 id;
  struct SerialTransferState *own;
  u32 gap;
  /* FAKEMATCH(temporary): the 1 kept in a halfword record, loaded from the pool
   * into r8 at entry. */
  struct SerialHalfword one;

  id = ~((REG_SIOCNT << 26) >> 30) & 1;
  one.v = 1;
  own = &gSerialTransfer;
  peer = (struct SerialTransferState *)gSerialPeerPayloads[id];
  if ((gLinkStatus & 3) != 3)
    return;
  {
    s32 *receive = &gSerialReceiveDest;
    s32 dest = *receive;
    if (dest != 0) {
      if (own->flags == 1 && (u8)(peer->peerFlags - 1) <= 2 - 1) {
        if (peer->active == (gSerialBlockNumber & 0x7f)) {
          own->active = 0;
          switch (peer->peerFlags) {
          case 1:
            Dma_Set(peer->payload, (void *)dest, DMA_COPY_WORDS(5), REG_DMA3);
            *receive += 20;
            gSerialReceivedSize += 20;
            own->status = (own->status + 1) | 0x80;
            break;
          case 2:
            Dma_Set(peer->payload, (void *)dest, DMA_COPY_WORDS(5), REG_DMA3);
            gSerialReceivedSize += 20;
            own->flags = 2;
            own->status = 0;
            own->active = one.v;
            break;
          }
          gSerialBlockNumber = (gSerialBlockNumber + 1) & 0x7f;
        } else if (gSerialBlockNumber & 0x80) {
          if (own->active & 0x80) {
            own->active = one.v;
          } else if (own->active == 1) {
            own->active = 0;
            gSerialBlockNumber &= 0x7f;
          }
        } else {
          own->active = gSerialBlockNumber | 0x80;
          gSerialBlockNumber |= 0x80;
        }
      } else {
        own->active = 0;
      }
    }
  }
  {
    s32 *send = &gSerialSendSource;
    s32 src = *send;
    if (src != 0) {
      if (peer->flags == 1) {
        if (peer->active & 0x80) {
          gap = (gSerialBlockNumber - peer->active) & 0x7f;
          *send = src - gap * 20;
          gSerialSendSize += gap * 20;
          gSerialBlockNumber -= gap;
          gSerialBlockNumber &= 0x7f;
        }
        if (gSerialSendSize != 0) {
          Dma_Set((void *)*send, own->payload, DMA_COPY_WORDS(5), REG_DMA3);
          gSerialSendSize += -20;
          if (gSerialSendSize != 0) {
            own->peerFlags = 1;
            own->active = gSerialBlockNumber & 0x7f;
            *send += 20;
          } else {
            own->peerFlags = 2;
            own->active = gSerialBlockNumber & 0x7f;
            *send += 20;
          }
          gSerialBlockNumber = (gSerialBlockNumber + 1) & 0x7f;
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

extern u8 gMapCellBuffer[];

/// An empty routine the link test calls to waste time.
void SerialTest_Delay(void) {}

/// The serial link test: sends the cartridge's first bytes or those at 0x1000
/// while A or B is held, and receives into the map cell buffer.
void SerialTest_Run(void) {
  u16 *tile;
  s32 value;
  s32 n;
  s32 delay;
  volatile u32 *held;

  Sound_PlayFar(MUS_003);
  Serial_Initialize();
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
  Serial_WaitForStatusMask(3);
restart:
  Serial_StartReceive(gMapCellBuffer);
  held = &gInput.held;
  for (;;) {
    if (*held & KEY_A)
      Serial_StartSend((void *)Cart_Start, 0x280);
    if (*held & KEY_B)
      Serial_StartSend((void *)(Rom_Start + 0x1000), 0x280);
    if (*held & KEY_START) {
      delay = 9999;
      do {
        delay--;
        SerialTest_Delay();
      } while (delay >= 0);
    }
    if (gSerialReceiveDest == 0) {
      Dma_Set(gMapCellBuffer, (void *)BG_SCREEN_BLOCK(2), DMA_COPY_WORDS(160),
              REG_DMA3);
      goto restart;
    }
    wait(1);
  }
}
