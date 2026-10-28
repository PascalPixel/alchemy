#include "local.h"
#include "call.h"

void Func_02004da0(s32 id, s32 x, s32 z);

void Func_02002868(void) {
  if (!GameFlag_Test(0x305)) {
    Actor_SetPartAttribute(Actor_GetById(11), 15);
    Func_02004da0(11, 136 << 16, 360 << 16);
    Actor_GetById(11)->scaleX = Actor_GetById(11)->scaleY = 0x1000;
  }
}
