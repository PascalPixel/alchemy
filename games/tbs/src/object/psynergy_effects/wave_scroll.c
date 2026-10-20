/// @file
/// The wavy background: scanline scroll offsets fed by HBlank DMA.
/// Part of psynergy_effects.c, compiled only through it.

#include "../psynergy_effects_internal.h"

/// Scheduler callback for the wavy background: restarts DMA 0 each frame to
/// copy the current page's BG1 to BG3 scroll offsets into the scroll registers
/// one scanline at a time. The first line is written directly; the DMA writes
/// three words at each HBlank from then on.
void DisplayScroll_ArmHBlankDma(void) {
  volatile u16 *dma;
  struct DisplayScrollWork *state;
  u32 *source;
  u16 status;
  u32 control;
  volatile u32 *destination;

  state = gHBlankScrollWork;
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

  // Enabled, at HBlank, repeating, 32-bit, destination reloaded: 3 words.
  control = 0xa6600003;
  Dma_Set(source, (void *)destination, control, (volatile u32 *)dma);
}

/// Scheduler callback for the wavy background: fills the hidden page with each
/// scanline's BG3, BG2 and BG1 offsets, the scroll plus a sine wave for x and
/// for y (an axis with no step stays flat), then shows that page.
void DisplayScroll_BuildAndSwapHBlankPage(void) {
  struct DisplayScrollWork *work = gHBlankScrollWork;
  s32 x3 = (s16)gBgScroll[3].x;
  s32 y3 = (s16)gBgScroll[3].y;
  s32 x2 = (s16)gBgScroll[2].x;
  s32 y2 = (s16)gBgScroll[2].y;
  s32 x1 = (s16)gBgScroll[1].x;
  s32 y1 = (s16)gBgScroll[1].y;
  u16 *row;
  s32 step;
  s32 phase;
  s32 amplitude;
  s32 scanline;
  u16 wave;

  row = &work->rows[work->page ^ 1][0][0].x;
  step = work->stepX;
  phase = (work->frame + (u16)y3) * work->freqX;
  if (step == 0) {
    for (scanline = 0; scanline != 160; scanline++) {
      row[scanline * 6] = x3;
      row[scanline * 6 + 2] = x2;
      row[scanline * 6 + 4] = x1;
    }
  } else {
    u16 a = x3;
    u16 b = x2;
    u16 c = x1;

    amplitude = work->ampX;
    for (scanline = 0; scanline != 160; scanline++) {
      wave = Iwram_MulQ16(gDisplayScroll_WaveSine[(phase >> 16) & 255],
                          amplitude) /
             256;
      *row = a + wave;
      row += 2;
      *row = b + wave;
      row += 2;
      *row = c + wave;
      row += 2;
      phase += step;
    }
  }
  row = &work->rows[work->page ^ 1][0][0].y;
  step = work->stepY;
  phase = (work->frame + (u16)y3) * work->freqY;
  if (step == 0) {
    for (scanline = 0; scanline != 160; scanline++) {
      row[scanline * 6] = y3;
      row[scanline * 6 + 2] = y2;
      row[scanline * 6 + 4] = y1;
    }
  } else {
    u16 a = y3;
    u16 b = y2;
    u16 c = y1;

    amplitude = work->ampY;
    for (scanline = 0; scanline != 160; scanline++) {
      wave = Iwram_MulQ16(gDisplayScroll_WaveSine[(phase >> 16) & 255],
                          amplitude) /
             256;
      *row = a + wave;
      row += 2;
      *row = b + wave;
      row += 2;
      *row = c + wave;
      row += 2;
      phase += step;
    }
  }
  work->frame++;
  work->page ^= 1;
}

// The shared source that starts the wavy background, at its place in the ROM.
#include "../../scene/common/scroll_initialize_hblank_dma.h"

/// Stops the wavy background: removes its two callbacks and stops DMA 0.
void DisplayScroll_DisarmHBlankDma(void) {
  volatile u16 *dma0;

  Scheduler_RemoveCallback(DisplayScroll_ArmHBlankDma);
  Scheduler_RemoveCallback(DisplayScroll_BuildAndSwapHBlankPage);
  dma0 = REG_DMA0;
  dma0[5] = (u16)~DMA_START_TIMING_DRQ_REPEAT & dma0[5];
  dma0[5] = (u16)~DMA_ENABLE & dma0[5];
  (void)dma0[5];
}
