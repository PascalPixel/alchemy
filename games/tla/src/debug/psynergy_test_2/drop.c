#include "types.h"
#include "object_runtime.h"
#include "call.h"

void Func_020005c8();
struct ObjectRuntime *Object_GetById();
void WaitFrames();

void Func_02000418(s32 mode, s32 id, s32 x, s32 z) {
  struct ObjectRuntime *object;

  object = Object_GetById(id);
  if (mode == 2) {
    Call3((void (*)())Func_020005c8, id, x << 16, z << 16);
    object->y = 0x200000;
    object->motionFlags = 3;
    object->gravity = 0x170a;
    Call1((void (*)())WaitFrames, 50);
  }
}
