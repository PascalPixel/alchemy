/// @file
/// Display, scroll and window HBlank DMA helpers.
/// Part of effect.c, the battle effect object, compiled only through it.

#include "../effect_internal.h"

/// Puts the battle screen back into the display state every effect starts
/// from and ends with: no blending, window 0 and window 1 both covering the
/// battle view (lines 16 to 135), and the effect display mode queued for the
/// next frame. Waits one frame.
void BattleFx_ConfigureEffectDisplay(void) {
  // Restore the battle viewport before the next effect changes its layers.
  REG_BLDCNT = 0;
  REG_BLDALPHA =
      BLEND_ALPHA(14, 16); // 14/16 of the top layer over all of the bottom
  EFFECT_SET_BATTLE_WINDOWS();
  // Window 1 starts as window 0 without BG1.
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = EFFECT_WINOUT;
  // Queues the effect display mode, 2 frames ahead.
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, 2 << 16);
  WaitFrames(1);
}

/// Scheduler callback: every scrollInterval frames, scrolls background 1 by
/// (scrollStepX, scrollStepY).
void BattleFx_AdvanceScrollOnInterval(void) {
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  u32 *timer = (u32 *)&work->scrollTimer;

  (*timer)++;
  if (*timer == work->scrollInterval) {
    gBgScroll[1].x += work->scrollStepX;
    gBgScroll[1].y += work->scrollStepY;
    *timer = 0;
  }
}

/// Scheduler callback: every scrollInterval frames, writes the effect's
/// background 2 reference point to the hardware and then moves it by
/// (scrollStepX, scrollStepY), so the affine layer slides in steps.
void BattleFx_AdvanceBg2OnInterval(void) {
  s32 timer;
  struct BattleEffectWork *work;

  work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  timer = work->scrollTimer + 1;
  work->scrollTimer = timer;
  if (timer == work->scrollInterval) {
    // Display the current reference before advancing to the next one.
    REG_BG2X = work->bg2x;
    REG_BG2Y = work->bg2y;
    work->bg2x += work->scrollStepX;
    work->bg2y += work->scrollStepY;
    work->scrollTimer = 0;
  }
}

/// Scheduler callback: restarts the HBlank DMA that copies one WIN0H value per
/// scanline from Ram_MapCellBuffer, so window 0's edges follow the per-line
/// table the effect drew there (a slanted or shaped window).
void BattleFx_ArmWin0HBlankDma(void) {
  volatile u16 *channel = REG_DMA0;
  // Stop the preceding scanline transfer before replacing its source.
  Dma_Stop(channel);
  Dma_Set(PARTICLES, (void *)&REG_WIN0H, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)channel);
}
