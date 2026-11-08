#include "types.h"
#include "actor.h"
#include "call.h"

void Func_020005c8();
struct Actor *Actor_GetById();
void wait();

void Func_02000418(s32 mode, s32 id, s32 x, s32 z) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 6 of 36 instructions
   * differ in tla-en. */
  struct Actor *object;

  object = Actor_GetById(id);
  if (mode == 2) {
    Func_020005c8(id, x << 16, z << 16);
    object->y = 0x200000;
    object->motionFlags = 3;
    object->gravity = SPLIT_CONSTANT(0x1700, 10);
    wait(50);
  }
}
