#include "types.h"
#include "call.h"

void GameFlag_SetBit(s32 flag);
void Func_02003aa4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02001fb0(void) {
  Call6((void (*)())Func_02003aa4, 121, 48, 1, 3, 123, 48);
  GameFlag_SetBit(0x250);
}

void Func_02001fd4(void) {
  Call6((void (*)())Func_02003aa4, 121, 48, 1, 3, 125, 48);
  GameFlag_SetBit(0x251);
}

void Func_02001ffc(void) {
  Call6((void (*)())Func_02003aa4, 108, 56, 1, 3, 109, 56);
  GameFlag_SetBit(0x252);
}
