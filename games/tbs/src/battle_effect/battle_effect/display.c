/// @file
/// Display, scroll and window HBlank DMA helpers.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// Puts the battle screen back into the display state every effect starts from
/// and ends with.
void BattleEffect_ConfigureEffectDisplay(void) {
  REG_BLDCNT = 0;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = EFFECT_WINOUT;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_16);
  wait(1);
}

/// Scrolls BG1 a step every few VBlanks.
void BattleEffect_AdvanceScrollOnInterval(void) {
  struct BattleEffectWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  u32 *timer = (u32 *)&work->scrollTimer;

  (*timer)++;
  if (*timer == work->scrollInterval) {
    gBgScroll[1].x += work->scrollStepX;
    gBgScroll[1].y += work->scrollStepY;
    *timer = 0;
  }
}

/// Slides the affine BG2 a step every few VBlanks.
void BattleEffect_AdvanceBg2OnInterval(void) {
  s32 timer;
  struct BattleEffectWork *work;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  timer = work->scrollTimer + 1;
  work->scrollTimer = timer;
  if (timer == work->scrollInterval) {
    REG_BG2X = work->bg2x;
    REG_BG2Y = work->bg2y;
    work->bg2x += work->scrollStepX;
    work->bg2y += work->scrollStepY;
    work->scrollTimer = 0;
  }
}

/// Restarts the DMA that feeds window 0's edges from the effect's per-line
/// table.
void BattleEffect_ArmWin0HBlankDma(void) {
  volatile u16 *channel = REG_DMA0;

  Dma_Stop(channel);
  Dma_Set(PARTICLES, (void *)&REG_WIN0H, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)channel);
}
