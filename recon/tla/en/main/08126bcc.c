// Draft, differ 56 of 130: advances the battle highlight while its mode is set:
// modes 6 and 4 fade the blend in (or out) over 32 frames at half steps, mode 2
// blends by the sixteen-step ramp, any other nonzero mode brightens by the
// ramp. ☀️'s Graphics_AdvancePaletteCycle in games/tbs/src/battle/battle.c has
// only the last case; the other three are ⚓️'s, written from the listing (the
// old draft had only the last). Register choice and order: the listing keeps
// the work pointer in r4 (a call-used register here) where this takes r0, so
// the step pointer and the clamped level trade r0 and r1 in the first case, and
// the BLDCNT address build and the pool loads sit a slot apart in each case.
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"

// The battle work block, as this function reads it.
struct HighlightWork {
  u8 unk000[0x676];
  u16 highlightStep;
  u16 highlightMode;
};

extern s8 gBattleFx_HighlightBlendRamp[];

void Graphics_AdvancePaletteCycle(void) {
  struct HighlightWork *battle =
      (struct HighlightWork *)Ram_HeapSlots->battleWork;
  u16 rampIndex;
  s32 nextIndex;
  u32 level;

  if (battle != NULL) {
    if (battle->highlightMode == 6) {
      level = battle->highlightStep >> 1;
      if (level > 16)
        level = 16;
      REG_BLDCNT = 0x3f40;
      REG_BLDALPHA = ((0x10 - level) << 8) | level;
      battle->highlightStep++;
    } else if (battle->highlightMode == 4) {
      level = battle->highlightStep >> 1;
      if (level > 16)
        level = 16;
      REG_BLDCNT = 0x3f40;
      REG_BLDALPHA = (level << 8) | (0x10 - level);
      battle->highlightStep++;
    } else if (battle->highlightMode == 2) {
      REG_BLDCNT = 0x3f40;
      level = gBattleFx_HighlightBlendRamp[battle->highlightStep];
      REG_BLDALPHA = (level << 8) | (0x10 - level);
      rampIndex = battle->highlightStep;
      nextIndex = (rampIndex + 1) & 0xf;
      if (rampIndex > 14)
        nextIndex |= 0x10;
      battle->highlightStep = nextIndex;
    } else if (battle->highlightMode != 0) {
      REG_BLDCNT = 0x3f90;
      REG_BLDALPHA = 0x10;
      REG_BLDY = gBattleFx_HighlightBlendRamp[battle->highlightStep];
      rampIndex = battle->highlightStep;
      nextIndex = (rampIndex + 1) & 0xf;
      if (rampIndex > 14)
        nextIndex |= 0x10;
      battle->highlightStep = nextIndex;
    }
  }
}
