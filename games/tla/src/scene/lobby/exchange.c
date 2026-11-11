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
#include "main_routine_work.h"
#include "serial.h"

s32 Func_0200039c(void);
s32 Func_02000578(void);

/// Resets the transfer state, as system/serial/reset_transfer.c does.
// In The Lost Age's store order.
static __inline__ void Lobby_ResetTransferState(void) {
  struct SerialTransferState *state;
  volatile u16 *ime;
  u32 saved;
  u32 zero;

  state = &gSerialTransfer;
  ime = &REG_IME;
  /* FAKEMATCH(dead): the IME exchange in a once-only block, which keeps it
   * before the zero and status constants; as plain statements, the constants
   * come first, 8 of 62 instructions of Func_020007b8 differ in tla-en. */
  do {
    saved = *ime;
    *ime = (u16)ime;
  } while (0);
  zero = 0;
  state = &gSerialTransfer;
  state->status = 0x80;
  *(u8 *)&state->peerFlags = zero;
  gSerialSendSource = zero;
  *(u8 *)&state->flags = zero;
  gSerialSendSize = zero;
  gSerialReceiveDest = zero;
  gSerialReceivedSize = zero;
  *ime = saved;
}

s32 Func_020007b8(void) {
  s32 result;
  s32 first;
  s32 flag;

  /* FAKEMATCH(dead): the result's zero in a once-only block, which keeps it
   * apart from the flag query that follows; as the plain result = 0, 2 of 62
   * instructions differ in tla-en. */
  do {
    result = 0;
  } while (0);
  flag = GameFlag_Test(0x302);
  gLinkExchangeState = result;
  if (!flag) {
    wait(5);
    result = Func_02000578();
    if (result < 0)
      goto fail;
    wait(5);
    first = result = Func_0200039c();
    if (result < 0)
      goto check;
  } else {
    first = result = Func_0200039c();
    if (result < 0)
      goto fail;
    wait(10);
    result = Func_02000578();
    if (result < 0)
      goto fail;
  }
  GameFlag_SetByte(0x3f0, first);
  result = first;
check:
  if (first < 0) {
  fail:
    Lobby_ResetTransferState();
  }
  return result;
}
