/// @file
/// Display, scroll and window HBlank DMA helpers.
// Part of f13ba50.c.

#include "../f13ba50_internal.h"
#include "module/battle_effect/f13ba50.h"

/// Puts the battle screen back into the display state the effects start
/// from and end with: no blending, window 0 and window 1 both covering the
/// battle view (lines 16 to 135), and the effect display mode queued.
/// Writes BLDCNT, BLDALPHA, the window registers and WININ and WINOUT,
/// queues DISPCNT for VBlank and waits one frame.
void BattleEffect_ConfigureEffectDisplay(void) {
  REG_BLDCNT = 0;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  REG_WIN0H = WIN_RANGE(0, 240);
  REG_WIN0V = WIN_RANGE(16, 136);
  REG_WIN1H = WIN_RANGE(0, 240);
  REG_WIN1V = WIN_RANGE(16, 136);
  REG_WININ = WININ_WIN1_CLR | WININ_WIN1_OBJ | WININ_WIN1_BG2 |
              WININ_WIN1_BG0 | WININ_WIN0_CLR | WININ_WIN0_OBJ |
              WININ_WIN0_BG2 | WININ_WIN0_BG1 | WININ_WIN0_BG0;
  REG_WINOUT = WINOUT_WINOBJ_ALL | WINOUT_WIN01_CLR | WINOUT_WIN01_BG0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG2_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  wait(1);
}

void Func_0813baec(void) {
  struct ScrollStepWork *work;

  work = (struct ScrollStepWork *)Ram_HeapSlots->battleFxWork[0];

  if (++work->counter == work->period) {
    gBgScroll[1].x += work->stepX;
    gBgScroll[1].y += work->stepY;
    work->counter = 0;
  }
}

/// A task: every period frames, writes the BG2 reference point to
/// the hardware, then moves it by (stepX, stepY).
void BattleEffect_AdvanceBg2OnInterval(void) {
  struct AffineStepWork *work;

  work = (struct AffineStepWork *)Ram_HeapSlots->battleFxWork[0];

  if (++work->counter == work->period) {
    REG_BG2X = work->x;
    REG_BG2Y = work->y;
    work->x += work->stepX;
    work->y += work->stepY;
    work->counter = 0;
  }
}

/// A task: restarts DMA0's HBlank copy of one WIN0H value per
/// scanline from the map cell buffer.
void BattleEffect_ArmWin0HBlankDma(void) {
  volatile u16 *dma = REG_DMA0;
  u16 control = dma[5];

  dma[5] = control & 0xc5ff; // Clear start timing and repeat.
  dma[5] &= 0x7fff;          // Stop.
  (void)dma[5];
  Dma_Set(Ram_MapCellBuffer, (void *)&REG_WIN0H, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)dma);
}
