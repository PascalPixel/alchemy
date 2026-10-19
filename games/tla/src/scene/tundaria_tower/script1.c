#include "types.h"

void Func_02003048(void);
void Func_02002fe8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_02002fc0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void GameFlag_SetBit(s32 a0);
void Func_02003050(void);

static inline void TundariaTower_02002fe8(s32 a0, s32 a1, s32 a2, s32 a3,
                                          s32 a4, s32 a5) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002fe8(a0, a1, a2, a3, a4, a5);
}

static inline void TundariaTower_02002fc0(s32 a0, s32 a1, s32 a2, s32 a3,
                                          s32 a4, s32 a5) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002fc0(a0, a1, a2, a3, a4, a5);
}

static inline void TundariaTower_GameFlag_SetBit(s32 a0) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(a0);
}

void Func_02000548(void) {
  Func_02003048();
  TundariaTower_02002fe8(19, 55, 7, 8, 24, 46);
  TundariaTower_02002fc0(24, 102, 24, 110, 7, 8);
  TundariaTower_GameFlag_SetBit(2370);
  Func_02003050();
}

void Func_0200058c(void) {
  Func_02003048();
  TundariaTower_02002fe8(37, 55, 7, 8, 32, 46);
  TundariaTower_02002fc0(32, 102, 32, 110, 7, 8);
  TundariaTower_GameFlag_SetBit(2371);
  Func_02003050();
}
