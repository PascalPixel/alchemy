/// @file
/// Display, scroll and window HBlank DMA helpers.
// Part of effect.c.

#include "../effect_internal.h"

/// Puts the battle screen back into the display state every effect starts
/// from and ends with: no blending, window 0 and window 1 both covering the
/// battle view (lines 16 to 135), and the effect display mode queued.
///
/// Called by the handlers that change the display themselves: at the start of
/// the beam sequence (beams.c), the particle streams and the summon scenes
/// Ramses, Kirin, Atalanta, Neptune, Boreas, Thor and Judgment, and at both
/// ends of Ice and Douse (falling.c). Writes BLDCNT, BLDALPHA, the window
/// registers and WININ and WINOUT, queues DISPCNT for two frames later and
/// waits one frame.
void BattleEffect_ConfigureEffectDisplay(void) {
  REG_BLDCNT = 0;
  // 14/16 of the top layer over all of the bottom, for when blending is on.
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  EFFECT_SET_BATTLE_WINDOWS();
  // Window 1 starts as window 0 without BG1.
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = EFFECT_WINOUT;
  // A delay of two frames (the queue's control word is delay << 16).
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, 2 << 16);
  WaitFrames(1);
}

/// Scrolls BG1 by (scrollStepX, scrollStepY) once every scrollInterval
/// frames.
///
/// Runs each frame as a scheduler callback added by BattleEffect_RunAtalanta
/// and BattleEffect_RunThor, which set the work block's scroll fields first and
/// remove it before they return. Counts work->scrollTimer and moves
/// gBgScroll[1].
void BattleEffect_AdvanceScrollOnInterval(void) {
  struct BattleEffectWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_ANIM);
  u32 *timer = (u32 *)&work->scrollTimer;

  (*timer)++;
  if (*timer == work->scrollInterval) {
    gBgScroll[1].x += work->scrollStepX;
    gBgScroll[1].y += work->scrollStepY;
    *timer = 0;
  }
}

/// Slides the affine BG2 in steps: once every scrollInterval frames, writes
/// the work block's reference point (bg2x, bg2y) to BG2X and BG2Y, then moves
/// it by (scrollStepX, scrollStepY).
///
/// Runs each frame as a scheduler callback added by
/// BattleEffect_RunParticleStreams, which removes it before it returns. Counts
/// work->scrollTimer.
void BattleEffect_AdvanceBg2OnInterval(void) {
  s32 timer;
  struct BattleEffectWork *work;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_ANIM);
  timer = work->scrollTimer + 1;
  work->scrollTimer = timer;
  if (timer == work->scrollInterval) {
    // The reference shown is the one before this step's move.
    REG_BG2X = work->bg2x;
    REG_BG2Y = work->bg2y;
    work->bg2x += work->scrollStepX;
    work->bg2y += work->scrollStepY;
    work->scrollTimer = 0;
  }
}

/// Restarts DMA 0 as an HBlank transfer of one WIN0H value a line from
/// PARTICLES (Ram_MapGridCellBuffer), so window 0's edges follow the per-line
/// table the effect drew there.
///
/// Runs each frame as a scheduler callback added by BattleEffect_RunIce and
/// BattleEffect_RunDouse, which remove it before they return. Stopping the
/// channel first makes each frame's transfer start again from the table's
/// first line.
void BattleEffect_ArmWin0HBlankDma(void) {
  volatile u16 *channel = REG_DMA0;

  Dma_Stop(channel);
  Dma_Set(PARTICLES, (void *)&REG_WIN0H, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)channel);
}
