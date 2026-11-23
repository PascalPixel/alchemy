/// @file
/// The line scroll: per-line scroll offsets fed by HBlank DMA.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// VBlank-pass task for the line scroll: restarts DMA 0 each VBlank to copy the
/// current page's BG1 to BG3 scroll offsets into the scroll registers one
/// scanline at a time.
void LineScroll_ArmDma(void) {
  volatile u16 *dma;
  struct LineScrollWork *state;
  u32 *source;
  u16 status;
  u32 control;
  volatile u32 *destination;

  state = gLineScroll;
  source = (u32 *)state->rows[state->page];

  dma = REG_DMA0;
  destination = (volatile u32 *)&REG_BG1HOFS;
  status = dma[5];
  control = status & (u16)~DMA_START_TIMING_DRQ_REPEAT;
  dma[5] = control;
  status = dma[5];
  control = status & (u16)~DMA_ENABLE;
  dma[5] = control;
  (void)dma[5];

  *destination = *source++;
  *destination = *source++;
  *destination = *source++;

  control = DMA_HBLANK_WORDS_PER_LINE(3);
  Dma_Set(source, (void *)destination, control, (volatile u32 *)dma);
}

/// Task for the line scroll: fills the hidden page with each scanline's BG3,
/// BG2 and BG1 offsets, the scroll plus a sine wave for x and for y (an axis
/// with no step stays flat), then shows that page.
void LineScroll_BuildPage(void) {
  struct LineScrollWork *work = gLineScroll;
  s32 x3 = (s16)gBgScroll[3].x;
  s32 y3 = (s16)gBgScroll[3].y;
  s32 x2 = (s16)gBgScroll[2].x;
  s32 y2 = (s16)gBgScroll[2].y;
  s32 x1 = (s16)gBgScroll[1].x;
  s32 y1 = (s16)gBgScroll[1].y;
  u16 *row;
  s32 step;
  FIXED phase;
  FIXED amplitude;
  s32 scanline;
  u16 wave;

  row = &work->rows[work->page ^ 1][0][0].x;
  step = work->stepX;
  phase = (work->frame + (u16)y3) * work->freqX;
  if (step == 0) {
    for (scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
      row[scanline * 6] = x3;
      row[scanline * 6 + 2] = x2;
      row[scanline * 6 + 4] = x1;
    }
  } else {
    u16 scroll3 = x3;
    u16 scroll2 = x2;
    u16 scroll1 = x1;

    amplitude = work->ampX;
    for (scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
      wave = Iwram_MulFX(gLineScroll_WaveSine[FIXEDtoINT(phase) &
                                              (LINE_SCROLL_WAVE_ENTRIES - 1)],
                         amplitude) /
             LINE_SCROLL_WAVE_SCALE;
      *row = scroll3 + wave;
      row += 2;
      *row = scroll2 + wave;
      row += 2;
      *row = scroll1 + wave;
      row += 2;
      phase += step;
    }
  }
  row = &work->rows[work->page ^ 1][0][0].y;
  step = work->stepY;
  phase = (work->frame + (u16)y3) * work->freqY;
  if (step == 0) {
    for (scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
      row[scanline * 6] = y3;
      row[scanline * 6 + 2] = y2;
      row[scanline * 6 + 4] = y1;
    }
  } else {
    u16 scroll3 = y3;
    u16 scroll2 = y2;
    u16 scroll1 = y1;

    amplitude = work->ampY;
    for (scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
      wave = Iwram_MulFX(gLineScroll_WaveSine[FIXEDtoINT(phase) &
                                              (LINE_SCROLL_WAVE_ENTRIES - 1)],
                         amplitude) /
             LINE_SCROLL_WAVE_SCALE;
      *row = scroll3 + wave;
      row += 2;
      *row = scroll2 + wave;
      row += 2;
      *row = scroll1 + wave;
      row += 2;
      phase += step;
    }
  }
  work->frame++;
  work->page ^= 1;
}

// The shared source that starts the line scroll, at its place in the ROM.
#include "../scene/shared/scroll_initialize_hblank_dma.c"

/// Stops the line scroll: removes its two tasks and stops DMA 0.
void LineScroll_Stop(void) {
  volatile u16 *dma0;

  Task_Remove(LineScroll_ArmDma);
  Task_Remove(LineScroll_BuildPage);
  dma0 = REG_DMA0;
  dma0[5] = (u16)~DMA_START_TIMING_DRQ_REPEAT & dma0[5];
  dma0[5] = (u16)~DMA_ENABLE & dma0[5];
  (void)dma0[5];
}
