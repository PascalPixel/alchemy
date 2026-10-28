#include "serial_runtime.h"
#include "io_reg.h"

void Serial_EnableTransferTimer(void) {
  struct SerialWork *state = &gSerialWork;

  if (state->mode != 0)
    state->transferEnabled = 1;
}

void Serial_DisableTransfer(void) {
  u32 mask;
  volatile u16 *ie;

  REG_IME = 0;
  /* FAKEMATCH(temporary): REG_IE's address in its own variable; as REG_IE,
   * 5 of 27 instructions differ in tla-en. */
  ie = &REG_IE;
  /* FAKEMATCH(one-pass): the rest in a one-pass block, so the mask's first
   * instruction goes before REG_IE's load; as plain statements, 2 of 27
   * instructions differ in tla-en. */
  do {
    /* FAKEMATCH(temporary): the mask set in its own variable first; as
     * *ie = *ie & 0xff3f, 2 of 27 instructions differ in tla-en. */
    mask = 0xff3f;
    mask = *ie & mask;
    *ie = mask;
    REG_IME = 1;
    REG_SIOCNT16 = 0x2003;
    REG_TM3CNT = 0x0000c963;
    REG_IF = 0xc0;
    gSerialWork.transferEnabled = 0;
  } while (0);
}
