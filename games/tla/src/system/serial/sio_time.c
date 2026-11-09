#include "serial.h"
#include "io_reg.h"

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
