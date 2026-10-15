#include "types.h"
#include "object_runtime.h"
#include "call.h"

struct ObjectRuntime *Object_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void Func_0200106c(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z);

void Func_02000440(void) {
  struct ObjectRuntime *object = Object_GetById(18);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  Call6((void (*)())Func_0200106c, 6, 16, 1, 1, 4, 16);
  GameFlag_SetBit(0x210);
}

void Func_02000478(void) {
  struct ObjectRuntime *object = Object_GetById(19);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  Call6((void (*)())Func_0200106c, 8, 30, 1, 1, 10, 30);
  GameFlag_SetBit(0x211);
}
