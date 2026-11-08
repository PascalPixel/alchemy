#include "../../../../../../games/tla/src/scene/taopo_swamp/local.h"

static __inline__ void Blend_SetAlpha(s32 value) {
  REG_BLDALPHA->value = value;
}

void Func_02002084(s32 fade) {
  Func_02002828();
  if (fade) {
    Blend_SetAlpha(SPLIT_CONSTANT(0xe00, 6));
    /* FAKEMATCH: the one-pass block finishes the blend value before the shared
     * register address; without it, 2 instructions differ in tla-en. */
    do {
    } while (0);
    wait(10);
    Blend_SetAlpha(SPLIT_CONSTANT(0xd00, 4));
    wait(10);
    Blend_SetAlpha(SPLIT_CONSTANT(0xd00, 2));
    wait(10);
    Blend_SetAlpha(SPLIT_CONSTANT(0xc00, 1));
    wait(10);
    Blend_SetAlpha(0xc00);
    wait(10);
  }
  Func_02001ff0(100, 50);
  Blend_SetAlpha(0xc07);
  if (gMainRoutineWork.entrance == 6)
    Task_Add(Func_02001eb8, 0x480);
}
