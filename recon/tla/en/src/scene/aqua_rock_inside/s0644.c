#include "../../../../../../games/tla/src/scene/aqua_rock_inside/local.h"
#include "version.h"

void Func_0200061c(struct Actor *object);

void Func_02000644(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 x;

  if (id == 9) {
    x = object->x >> 20;
    if (x == 39) {
#if !TLA_PATCH_1
      object->motionFlags = 3;
      wait(15);
      object->motionFlags = 0;
      GameFlag_SetBit(0x95c);
      Func_02003d1c(41, 9, 1, 1, x, id);
#else
      Func_02003d7c();
      Func_02003eac(0);
      Func_0200061c(object);
      GameFlag_SetBit(SPLIT_CONSTANT(0x900, 92));
      Func_02003d1c(41, 9, 1, 1, x, id);
      Func_02003d84();
#endif
    }
  }
}
