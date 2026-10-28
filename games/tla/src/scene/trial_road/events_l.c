#include "trial_road.h"

void Func_020007e8(void) {
  if (Actor_GetById(16)->z >> 20 == 23)
    Func_020059c0();
  else
    Func_02005918(gMainRoutineWork.currentOwner, 6, 0);
}

void Func_02000818(s32 a, s32 id) {
  if ((u32)(id - 15) <= 1)
    Func_02000660(id);
}
