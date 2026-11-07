#include "types.h"
#include "io_reg.h"
#include "system.h"

/// The Broken Seal's BattleBackground_FadeAlphaOut: blends BG2 over the
/// backdrop and steps BG2's weight down by 2 a frame from 15, while the
/// backdrop's stays 16.
void Func_08126944(void) {
  s32 step;

  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BD;
  for (step = 1; step <= 16; step += 2) {
    // Arithmetic on BLEND_ALPHA, not the macro's | form, which changes the
    // code.
    REG_BLDALPHA = BLEND_ALPHA(16, 16) - step;
    wait(1);
  }
}
