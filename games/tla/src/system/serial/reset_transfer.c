#include "types.h"
#include "io_reg.h"
#include "serial.h"

s32 Serial_GetActiveTransfers(void) {
  s32 flags;

  flags = 0;
  if (gSerialSendSource != 0)
    flags = 1;
  if (gSerialReceiveDest != 0)
    flags |= 2;
  return flags;
}

void BattleLink_ResetTransferState(void) {
  struct SerialTransferState *state;
  volatile u16 *ime;
  u32 saved;
  u32 zero;

  state = &gSerialTransfer;
  ime = &REG_IME;
  // The IME exchange in a once-only block keeps it before the zero and the
  // status store; as plain statements, 10 of 19 instructions differ in tla-en.
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
