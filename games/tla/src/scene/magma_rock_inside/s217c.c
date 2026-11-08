#include "local.h"
#include "call.h"

void Func_02004da0(s32 id, s32 x, s32 z);

void Func_0200217c(void) {
  if (!GameFlag_Test(0x303)) {
    Actor_SetPartAttribute(Actor_GetById(21), 15);
    Func_02004da0(21, PIXELS(600), PIXELS(312));
    Actor_GetById(21)->scaleX = Actor_GetById(21)->scaleY = 0x1000;
  }
}
