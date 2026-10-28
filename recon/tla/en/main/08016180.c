// Draft, differ 8 of 127 in tla-en under -mthumb-slow-loads (2026-10-26). A
// device search (each statement followed by an empty asm or a one-pass block,
// wrapped in one, or swapped with the next, kept when it helps) reaches 4 with
// an empty asm after `REG_IME = 0;`; an empty asm after `REG_IME = 0;`, and
// finds nothing closer.
// Draft, differ 8 of 127: three one-slot swaps around the interrupt
// registers. The native stores REG_IME = 0 before it loads &REG_IE (twice),
// and loads 0x4003 before it reads SIOCNT. Measured: an empty asm after the
// first REG_IME = 0 orders that pair but swaps the first call's arguments
// (still 8); after the second it moves the REG_IME = 1 constant up instead;
// a one-pass block there is differ 12; four minutes of permuter found
// differ 6 only by wrapping the tail in a one-pass block.
#include "serial_runtime.h"
#include "dma.h"

void Serial_Initialize(void) {
  u32 interruptEnable;
  /* FAKEMATCH: keep the fill word's address before its store. */
  volatile u32 zero;
  s32 index;
  volatile u16 *imeReg;
  struct SerialWork *state;

  interruptEnable = REG_IME;
  /* FAKEMATCH: retain the reference's IME pointer lifetime. */
  do {
    imeReg = &REG_IME;
  } while (0);
  *imeReg = (u32)imeReg;
  Runtime_SetIrqHandler(7, 0, Serial_HandleTransferInterrupt);
  Runtime_SetIrqHandler(6, 0, Serial_HandleTransferInterrupt);
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
  zero = 0;
  Dma_Set(&zero, state, 0x85000058, (volatile u32 *)0x040000d4);
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
  *(volatile u16 *)&gSerialExchangeActive = 1;
  gLinkExchangeState = 0;
  gSerialSendSource = 0;
  gSerialSendSize = 0;
  gSerialReceiveDest = 0;
  gSerialReceivedSize = 0;
  BattleLink_ResetTransferState();
  REG_IME = interruptEnable;
}
