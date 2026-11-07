#include "types.h"

void GameFlag_SetBit(s32 a0);
void Func_020030f0(s32 a0, s32 a1);

void Func_02000280(void) {
  GameFlag_SetBit(513);
  Func_020030f0(8, 52);
}
