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
#include "party_state.h"
#include "serial_runtime.h"

s32 Func_0200039c(void);
s32 Func_02000578(void);

static __inline__ void Flag_SetByte(s32 flag, s32 value) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  GameFlag_SetByte(flag, value);
}

// ⚓️ の順で書く転送状態のリセット（system/link/reset_transfer.c と同じ内容）
static __inline__ void Lobby_ResetTransferState(void) {
  struct SerialTransferState *state;
  volatile u16 *ime;
  u32 saved;
  u32 zero;

  state = &gSerialTransfer;
  ime = &REG_IME;
  /* FAKEMATCH: the one-pass block keeps the IME exchange before the zero and
   * status constants. */
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

  /* FAKEMATCH: separate initialization from the following flag query. */
  do {
    result = 0;
  } while (0);
  flag = GameFlag_Test(0x302);
  gLinkExchangeState = result;
  if (!flag) {
    WaitFrames(5);
    result = Func_02000578();
    if (result < 0)
      goto fail;
    WaitFrames(5);
    first = result = Func_0200039c();
    if (result < 0)
      goto check;
  } else {
    first = result = Func_0200039c();
    if (result < 0)
      goto fail;
    WaitFrames(10);
    result = Func_02000578();
    if (result < 0)
      goto fail;
  }
  Flag_SetByte(0x3f0, first);
  result = first;
check:
  if (first < 0) {
  fail:
    Lobby_ResetTransferState();
  }
  return result;
}
