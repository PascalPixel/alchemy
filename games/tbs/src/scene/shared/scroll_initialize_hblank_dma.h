#include "mem.h"
#include "engine_call.h"
#include "dma.h"
#include "system.h"
#include "task.h"
#include "scroll.h"
#include "io_reg.h"

// The Lemurian ship: the ripple of the water, a wave in each axis of the
// background's scrolling, changed line by line in the horizontal blank.

static __inline__ void WaitDma(volatile u32 *channel) {
  while (channel[2] & 0x80000000) {
  }
}

/// Starts the ripple: its work (heap block 34) holds the mode and, for x and y,
/// the wave's frequency, its step each frame and its amplitude; two
/// tasks build each frame's table of line offsets and sends it by DMA in
/// the horizontal blank.
void ScanlineEffect_InitWave(s32 mode, s32 freqX, s32 stepX, s32 ampX,
                             s32 freqY, s32 stepY, s32 ampY) {
  struct ScanlineEffectWork *work;

  work = AllocHeapSlotEwram(HEAP_SLOT_SCANLINE_EFFECT, 0xf20);
  Dma_Fill32(0, work, 0xf20);
  WaitDma(REG_DMA3);
  work->mode = mode;
  work->freqX = freqX;
  work->freqY = freqY;
  work->ampX = ampX;
  work->ampY = ampY;
  work->stepX = stepX;
  work->stepY = stepY;
  Task_AddFrame(ScanlineEffect_BuildPage, TASK_PASS_FRAME);
  Task_AddFrame(ScanlineEffect_ArmDma, TASK_PASS_VBLANK);
}
