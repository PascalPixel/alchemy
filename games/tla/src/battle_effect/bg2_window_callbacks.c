#include "types.h"
#include "ram_buffer.h"
#include "dma.h"

struct AffineStepWork {
  u8 unk0000[0x7790];
  s32 counter;
  s32 period;
  s32 stepX;
  s32 stepY;
  u8 unk77a0[0x30];
  s32 x;
  s32 y;
};

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
