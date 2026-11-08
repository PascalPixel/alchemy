/// @file
/// Steps the battle highlight's blend.

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

/// Advances the battle highlight while its mode is set: modes 6 and 4 fade the
/// blend in or out over 32 frames at half steps, mode 2 blends by the
/// sixteen-step ramp, and any other nonzero mode brightens by the ramp.
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
