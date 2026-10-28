#include "types.h"
#include "actor.h"
#include "call.h"

void Func_020005c8();
struct Actor *Actor_GetById();
void wait();

void Func_02000418(s32 mode, s32 id, s32 x, s32 z) {
  struct Actor *object;

  object = Actor_GetById(id);
  if (mode == 2) {
    Call3((void (*)())Func_020005c8, id, x << 16, z << 16);
    object->y = 0x200000;
    object->motionFlags = 3;
    object->gravity = 0x170a;
    Call1((void (*)())wait, 50);
  }
}
