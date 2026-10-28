#include "types.h"
#include "actor.h"
#include "call.h"

void Func_02000df8();
struct Actor *Actor_GetById();
void WaitFrames();

void Func_02000ae0(s32 mode, s32 id, s32 x, s32 z) {
  struct Actor *object;
  s32 scale;

  object = Actor_GetById(id);
  if (mode == 1) {
    Func_02000df8(id, x << 16, z << 16);
    object->scaleX = 0;
    object->scaleY = 0;
  }
  if (mode == 2) {
    /* FAKEMATCH: ⚓️ loads the scale before it builds the bound; without the
     * barrier the bound's first byte comes first. */
    __asm__ volatile("" : "+l"(object));
    while (object->scaleX < 0x10000) {
      WaitFrames(1);
      scale = object->scaleX;
      /* FAKEMATCH: and loads it again before it builds the step. */
      __asm__ volatile("" : "+l"(scale));
      scale += 0x51e;
      object->scaleX = scale;
      object->scaleY = scale;
    }
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
  }
}

void Func_02000b50(s32 mode, s32 id, s32 x, s32 z) {
  struct Actor *object;

  object = Actor_GetById(id);
  if (mode == 2) {
    Call3((void (*)())Func_02000df8, id, x << 16, z << 16);
    object->y = 0x200000;
    object->motionFlags = 3;
    object->gravity = 0x170a;
    Call1((void (*)())WaitFrames, 50);
  }
}
