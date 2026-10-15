#include "mem.h"
#include "engine_call.h"
#include "dma.h"
#include "system.h"
#include "callback_scheduler.h"
#include "scroll.h"
#include "io_reg.h"

static __inline__ void WaitDma(volatile u32 *channel) {
  while (channel[2] & 0x80000000) {
  }
}

void DisplayScroll_InitializeHBlankDma(s32 mode, s32 freqX, s32 stepX, s32 ampX,
                                       s32 freqY, s32 stepY, s32 ampY) {
  struct DisplayScrollWork *work;

  work = Runtime_AllocateBlock(34, 0xf20);
  Dma_Fill32(0, work, 0xf20);
  WaitDma(REG_DMA3);
  work->mode = mode;
  work->freqX = freqX;
  work->freqY = freqY;
  work->ampX = ampX;
  work->ampY = ampY;
  work->stepX = stepX;
  work->stepY = stepY;
  FieldScheduler_AddCallback(DisplayScroll_BuildAndSwapHBlankPage, 3200);
  FieldScheduler_AddCallback(DisplayScroll_ArmHBlankDma, 1152);
}
