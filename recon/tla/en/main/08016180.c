// Near miss, differ 8 of 127 (tla-en, 2026-10-31): scheduling order only, in
// the Broken Seal's form with no device. After each REG_IME = 0 the native
// stores IME before it loads &REG_IE (the second time also 0x80); sched2
// gives &REG_IE's load priority 14 (92 the second time) over the store's 12
// (90), with no dependence between them that the C can add.
// Measured in this form: an empty asm after the first REG_IME = 0 orders
// that pair but swaps the first call's r1 and r2 setup (6); an empty one-pass
// block after the second (or one around REG_IE |= 0x80) orders that pair
// but moves gLinkExchangeState's zero above the IME store (6); both together
// differ 4. A one-pass block around the first REG_IME = 0 or REG_IE &= 0xff3f
// differs 13 to 15; REG_IE through its own pointer with the mask set first,
// as Serial_DisableTransfer, differs 8. Four minutes of permuter found only
// the empty one-pass block (6).
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
