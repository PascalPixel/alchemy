/// @file
/// The link cable: payload exchange, block transfers and the link test.
// Part of system.c.

#include "../system_internal.h"
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

void Serial_EnableTransferTimer(void) {
  struct SerialWork *state = &gSerialWork;

  if (state->mode != 0)
    state->transferEnabled = 1;
}

void Serial_DisableTransfer(void) {
  REG_IME = 0;
  REG_IE &= (u16)~(INTR_FLAG_SERIAL | INTR_FLAG_TIMER3);
  REG_IME = 1;
  REG_SIOCNT16 = SIO_MULTI_MODE | SIO_115200_BPS;
  REG_TM3CNT = 0x0000c963;
  REG_IF = 0xc0;
  gSerialWork.transferEnabled = 0;
}

s32 Serial_PollAndTransfer(void *send, void *receive) {
  volatile u32 *siocnt;
  u32 control;
  s32 status;
  s32 result;

  siocnt = &REG_SIOCNT;
  control = *siocnt;
  switch (gSerialWork.phase) {
  case 0: {
    u8 multiplayerId = control & 0x30;
    u8 mode;

    if (multiplayerId == 0 && (mode = control & 0x88) == 8) {
      u8 serialError = control & 4;

      if (serialError == 0 && gSerialWork.sendIndex == -1) {
        u32 ie;

        Io_Write16(0, &REG_IME);
        ie = REG_IE;
        Io_Write16((ie & ~0x80) | 0x40, &REG_IE);
        Io_Write16(1, &REG_IME);
        *((volatile s8 *)siocnt + 1) &= ~0x40;
        REG_IF = 0xc0;
        REG_TM3CNT = 0xc963;
        gSerialWork.mode = mode;
      }
    } else if (multiplayerId == 0) {
      break;
    }
    gSerialWork.phase = 1;
  }
  case 1:
    Serial_CollectReceivedPayloads(receive);
    Serial_PrepareSendPacket(send);
    break;
  }

  gSerialWork.packetNumber++;
  status = gSerialWork.currentMask | (gSerialWork.receivedMask << 8);
  if (gSerialWork.mode == 8)
    status |= 0x80;
  result = Serial_AddErrorFlag(status);
  if (((control << 26) >> 30) > 1)
    result |= 0x2000;
  return result;
}

s32 Serial_PollStatus(void) {
  volatile u32 *siocnt;
  u32 control;
  s32 status;
  s32 result;

  siocnt = &REG_SIOCNT;
  control = *siocnt;
  if (gSerialWork.phase == 0) {
    u8 mode = control & 0x88;

    if (mode == 8) {
      u8 serialError = control & 4;

      if (serialError == 0 && gSerialWork.sendIndex == -1) {
        u32 ie;

        Io_Write16(0, &REG_IME);
        ie = REG_IE;
        Io_Write16((ie & ~0x80) | 0x40, &REG_IE);
        Io_Write16(1, &REG_IME);
        *((volatile s8 *)siocnt + 1) &= ~0x40;
        REG_IF = 0xc0;
        REG_TM3CNT = 0xc963;
        gSerialWork.mode = mode;
      }
      gSerialWork.phase = 1;
    }
    gSerialWork.packetNumber++;
  }

  status = gSerialWork.currentMask | (gSerialWork.receivedMask << 8);
  if (gSerialWork.mode == 8)
    status |= 0x80;
  result = Serial_AddErrorFlag(status);
  if (((control << 26) >> 30) > 1)
    result |= 0x2000;
  return result;
}

/// In phase 1, collects incoming data in receive and builds the next packet
/// from send. Returns the link status: bits 0-7 currentMask (with bit 7 also
/// set in mode 8), bits 8-15 receivedMask, 0x1000 for the parent and 0x2000
/// when the SIOCNT multiplayer ID is at least 2.
s32 Serial_ExchangePayloads(void *send, void *receive) {
  volatile u32 *siocnt;
  u32 cnt;
  s32 st;
  s32 ret;

  siocnt = &REG_SIOCNT;
  cnt = *siocnt;
  if (gSerialWork.phase == 1) {
    Serial_CollectReceivedPayloads(receive);
    Serial_PrepareSendPacket(send);
    gSerialWork.packetNumber++;
  }
  st = gSerialWork.currentMask | (gSerialWork.receivedMask << 8);
  if (gSerialWork.mode == 8)
    st |= 0x80;
  ret = Serial_AddErrorFlag(st);
  if (((cnt << 26) >> 30) > 1) // Bits 4 and 5.
    ret |= 0x2000;
  return ret;
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

u8 Serial_CollectReceivedPayloads(void *payload) {
  u8 flags[8];
  s32 i;
  u32 k;
  u16 *packet;
  u16 *swap;
  s32 sum;

  *(u32 *)&flags[4] = 0;
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
    for (k = 0; k <= 13; k++)
      sum += packet[k];
    if (flags[i] == 1 && (s16)sum == -1) {
      Dma_Set(gSerialWork.pendingBuffer[i] + 2, (u8 *)payload + i * 24,
              DMA_COPY_WORDS(6), REG_DMA3);
      gSerialWork.currentMask |= 1 << i;
    }
    if ((s16)sum == -1)
      gSerialWork.pendingBuffer[i][1] = ~gSerialWork.pendingBuffer[i][1];
  }
  gSerialWork.receivedMask |= gSerialWork.currentMask;
  return gSerialWork.currentMask;
}

/// The serial and timer 3 interrupt: each one moves a halfword. It sends the
/// next halfword of the outgoing packet (0xfefe between packets) and stores
/// the halfword received from each console; a console's fourteenth halfword
/// completes its packet. The parent then restarts timer 3 for the next.
// As ☀️'s Serial_HandleTransferInterrupt.
void Serial_HandleTransferInterrupt(void) {
  struct SerialWork *sendState;
  struct SerialWork *receiveState;
  struct SerialWork *tailState;
  u16 recv[4];
  volatile u32 *sioControl;
  s32 channel;

  sioControl = &REG_SIOCNT;
  *(u64 *)recv = *(volatile u64 *)REG_SIODATA32;
  sendState = &gSerialWork;
  {
    s32 index;
    u8 transferError = (*sioControl << 25) >> 31;

    index = sendState->sendIndex;
    sendState->transferError = transferError;
    if (index == -1) {
      u32 idleWord = 0xfefe;
      u16 *swap;

      ((volatile u16 *)sioControl)[1] = idleWord;
      swap = sendState->sendBuffer[1];
      sendState->sendBuffer[1] = sendState->sendBuffer[0];
      sendState->sendBuffer[0] = swap;
    } else if (index >= 0) {
      ((volatile u16 *)sioControl)[1] = sendState->sendBuffer[1][index];
    }
  }
  receiveState = &gSerialWork;
  if (receiveState->sendIndex <= 14)
    receiveState->sendIndex++;

  // The two receive channels share one backedge.
  channel = 0;
receive_loop:
  {
    s32 receiveIndex;
    u16 *incoming;

    if (recv[channel] == 0xfefe && receiveState->receiveIndex[channel] > 13) {
      receiveState->receiveIndex[channel] = -1;
    } else {
      receiveIndex = receiveState->receiveIndex[channel];
      incoming = receiveState->incomingBuffer[channel];
      incoming[receiveIndex] = recv[channel];
      if (receiveIndex == 13) {
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
  if (tailState->receiveIndex[channel] <= 14)
    tailState->receiveIndex[channel]++;
  channel++;
  receiveState = tailState;
  if (channel <= 1)
    goto receive_loop;

  if (tailState->mode == 8) {
    REG_TM3CNT_H = 0;
    REG_SIOCNT16 |= 0x80;
    REG_TM3CNT_H = TIMER_INTR_ENABLE | TIMER_ENABLE;
  }
}

void Serial_RemoveIrqHandlers(void) {
  gSerialExchangeActive = 0;
  Irq_SetHandler(7, 0, 0);
  Irq_SetHandler(6, 0, 0);
}

u32 Serial_WaitForStatusMask(s32 mask) {
  if ((mask & gLinkStatus) != mask) {
    do {
      wait(1);
    } while ((mask & gLinkStatus) != mask);
  }
  return (REG_SIOCNT << 26) >> 30;
}

s32 Serial_StartSend(void *source, s32 size) {
  struct SerialTransferState *state = &gSerialTransfer;
  u32 saved;

  if (gSerialSendSource != 0)
    return -1;
  IRQ_LOCK(saved);
  state->status = 0x80;
  gSerialSendSource = (s32)source;
  gSerialSendSize = size;
  gSerialBlockNumber = 0;
  state->active = 1;
  IRQ_UNLOCK(saved);
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
      Serial_StartSend((void *)Cart_Restart, 0x280);
    if (*held & KEY_B)
      Serial_StartSend((void *)(Cartridge_Restart + 0x1000), 0x280);
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
