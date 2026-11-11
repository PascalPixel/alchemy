#include "local.h"

static __inline__ void Blend_SetAlpha(s32 value) {
  REG_BLDALPHA->value = value;
}

void Func_02002084(s32 fade) {
  Func_02002828();
  if (fade) {
    Blend_SetAlpha(0xe06);
    /* FAKEMATCH(dead): the empty once-only block finishes the blend value
     * before the shared register address; without it, 2 of 60 instructions
     * differ in tla-en. */
    do {
    } while (0);
    System_Wait(10);
    Blend_SetAlpha(0xd04);
    System_Wait(10);
    Blend_SetAlpha(0xd02);
    System_Wait(10);
    Blend_SetAlpha(0xc01);
    System_Wait(10);
    Blend_SetAlpha(0xc00);
    System_Wait(10);
  }
  Func_02001ff0(100, 50);
  Blend_SetAlpha(0xc07);
  if (gMainRoutineWork.entrance == 6)
    FieldTask_AddFrame(Func_02001eb8, 0x480);
}
