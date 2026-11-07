#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "io_reg.h"
#include "serial.h"
#include "irq_lock.h"

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
    /* FAKEMATCH(barrier): an empty asm after the send index's read, so it
     * loads before the transferError store; as plain statements, 2 of 121
     * instructions differ in tla-en. */
    asm volatile("");
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
    volatile u16 *sio = &REG_SIOCNT16;
    /* FAKEMATCH(pin): the 0x80 a halfword pinned to r2 behind a barrier, so
     * SIOCNT's address loads before the timer store and the 0x80 before
     * SIOCNT is read; as REG_SIOCNT16 |= 0x80 after an empty asm, 2 of 121
     * instructions differ in tla-en. */
    register u16 start asm("r2");

    REG_TM3CNT_H = 0;
    start = 0x80;
    asm volatile("" : "+r"(start));
    *sio |= start;
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
