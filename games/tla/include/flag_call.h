#ifndef TLA_FLAG_CALL_H
#define TLA_FLAG_CALL_H

#include "types.h"

// フラグ操作のスクリプト層
// A scene builds its flag id again for every command: each call goes
// through an inline, as the ☀️ scene script layer does.

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);

static inline s32 Flag_IsSet(s32 flag) {
  /* FAKEMATCH: as in ☀️'s scene layer, the inline parameter builds the flag id
   * again at each call; direct calls share one register */
  return GameFlag_Test(flag);
}

static inline void Flag_Set(s32 flag) {
  /* FAKEMATCH: as in ☀️'s scene layer, the inline parameter builds the flag id
   * again at each call; direct calls share one register */
  GameFlag_SetBit(flag);
}

static inline void Flag_Clear(s32 flag) {
  /* FAKEMATCH: as in ☀️'s scene layer, the inline parameter builds the flag id
   * again at each call; direct calls share one register */
  GameFlag_ClearBit(flag);
}

#endif
