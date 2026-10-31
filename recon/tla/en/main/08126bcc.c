// Near miss, differ 4 of 130 (tla-en, 2026-10-31; 48 before): advances the
// battle highlight while its mode is set: modes 6 and 4 fade the blend in (or
// out) over 32 frames at half steps, mode 2 blends by the sixteen-step ramp,
// any other nonzero mode brightens by the ramp. ☀️'s twin,
// BattleBackground_StepHighlightBlend in games/tbs/src/battle/battle/
// background.c, has only the last case; the other three are ⚓️'s, written
// from the listing. The level local in each fade's block and the ramp entry
// read twice for the blend give the native's registers (48 -> 4). Left,
// scheduling only: in the two ramp cases the native builds the 15 mask
// between the REG_BLDALPHA (or REG_BLDY) store and the reload of the step,
// where alchemy-gcc reloads first (the known store, other instruction, load
// pattern; permuter 6 minutes found nothing). The sched2 dump shows why no C
// moves it: the reload depends on that store at cost 1, since
// -mthumb-slow-loads adds its wait only for a load from the store's own
// address, so both are ready in the same cycle and the load's priority (5,
// against the mask's 2) puts it first. ☀️'s twin reloads first in its ROM.
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"

// The battle work block, as this function reads it.
struct HighlightWork {
  u8 unk000[0x676];
  u16 highlightStep;
  u16 highlightMode;
};

extern s8 gBattleEffect_HighlightBlendRamp[];

void Graphics_AdvancePaletteCycle(void) {
  struct HighlightWork *battle =
      (struct HighlightWork *)Ram_HeapSlots->battleWork;

  if (battle != NULL) {
    if (battle->highlightMode == 6) {
      u32 level = battle->highlightStep >> 1;

      if (level > 16)
        level = 16;
      REG_BLDCNT = 0x3f40;
      REG_BLDALPHA = ((0x10 - level) << 8) | level;
      battle->highlightStep++;
    } else if (battle->highlightMode == 4) {
      u32 level = battle->highlightStep >> 1;

      if (level > 16)
        level = 16;
      REG_BLDCNT = 0x3f40;
      REG_BLDALPHA = (level << 8) | (0x10 - level);
      battle->highlightStep++;
    } else if (battle->highlightMode == 2) {
      REG_BLDCNT = 0x3f40;
      REG_BLDALPHA =
          (gBattleEffect_HighlightBlendRamp[battle->highlightStep] << 8) |
          (0x10 - gBattleEffect_HighlightBlendRamp[battle->highlightStep]);
      battle->highlightStep = ((battle->highlightStep + 1) & 0xf) |
                              (battle->highlightStep > 14 ? 0x10 : 0);
    } else if (battle->highlightMode != 0) {
      REG_BLDCNT = 0x3f90;
      REG_BLDALPHA = 0x10;
      REG_BLDY = gBattleEffect_HighlightBlendRamp[battle->highlightStep];
      battle->highlightStep = ((battle->highlightStep + 1) & 0xf) |
                              (battle->highlightStep > 14 ? 0x10 : 0);
    }
  }
}
