#include "serial.h"
#include "dma.h"
#include "irq_lock.h"

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
  BattleLink_ResetTransferState();
  IRQ_UNLOCK(interruptEnable);
}
