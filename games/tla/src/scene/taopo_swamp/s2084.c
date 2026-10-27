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
    WaitFrames(10);
    Blend_SetAlpha(0xd04);
    WaitFrames(10);
    Blend_SetAlpha(0xd02);
    WaitFrames(10);
    Blend_SetAlpha(0xc01);
    WaitFrames(10);
    Blend_SetAlpha(0xc00);
    WaitFrames(10);
  }
  Func_02001ff0_(100, 50);
  Blend_SetAlpha(0xc07);
  if (gPartyState.entrance == 6)
    Scheduler_AddOrUpdateCallback(Func_02001eb8, 0x480);
}
