/// @file
/// The Lemurian ship: the ripple of the water, a wave in each axis of the
/// background's scrolling, changed line by line in the horizontal blank.

#include "system/system_call.h"

static inline void WaitDma(volatile u32 *channel) {
  while (channel[2] & DMA_BUSY) {
  }
}

/// Starts the ripple, with the wave's frequency, its step each frame and its
/// amplitude for x and for y: one task builds each frame's table of line
/// offsets, and another sends it by DMA in the horizontal blank.
void LineScroll_InitWave(s32 mode, FIXED freqX, s32 stepX, FIXED ampX,
                         FIXED freqY, s32 stepY, FIXED ampY) {
  struct LineScrollWork *work;

  work =
      HeapSlot_AllocEwram(HEAP_SLOT_LINE_SCROLL, sizeof(struct LineScrollWork));
  Dma_Fill32(0, work, sizeof(struct LineScrollWork));
  WaitDma(REG_DMA3);
  work->mode = mode;
  work->freqX = freqX;
  work->freqY = freqY;
  work->ampX = ampX;
  work->ampY = ampY;
  work->stepX = stepX;
  work->stepY = stepY;
  Task_AddFrame(LineScroll_BuildPage, TASK_PASS_FRAME);
  Task_AddFrame(LineScroll_ArmDma, TASK_PASS_VBLANK);
}
