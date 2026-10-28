#include "local.h"

static __inline__ void Blend_SetAlpha(s32 value) {
  REG_BLDALPHA->value = value;
}

void Func_02002084(s32 fade) {
  Func_02002828();
  if (fade) {
    Blend_SetAlpha(0xe06);
    /* FAKEMATCH: the one-pass block finishes the blend value before the shared
     * register address; without it, 2 instructions differ in tla-en. */
    do {
    } while (0);
    wait(10);
    Blend_SetAlpha(0xd04);
    wait(10);
    Blend_SetAlpha(0xd02);
    wait(10);
    Blend_SetAlpha(0xc01);
    wait(10);
    Blend_SetAlpha(0xc00);
    wait(10);
  }
  Func_02001ff0_(100, 50);
  Blend_SetAlpha(0xc07);
  if (gAdventure.entrance == 6)
    Task_Add(Func_02001eb8, 0x480);
}
