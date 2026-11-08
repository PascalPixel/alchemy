#include "types.h"
#include "actor.h"
#include "call.h"

struct Actor *Actor_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void Func_0200106c(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z);

void Func_020000ec(void) {
  struct Actor *object = Actor_GetById(11);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  Call6((void (*)())Func_0200106c, 24, 32, 1, 1, 22, 32);
  GameFlag_SetBit(0x221);
}

void Func_02000124(void) {
  Func_0200106c(30, 35, 1, 1, 28, 34);
  Func_0200106c(30, 34, 1, 1, 28, 33);
  GameFlag_SetBit(0x222);
}
