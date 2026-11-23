/// @file
/// The line scroll: per-line scroll offsets fed by HBlank DMA.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "fixed_math.h"
#include "flags.h"
#include "sounds.h"
#include "field/field.h"

/// LineScrollWork.mode: 0 waves BG1, BG2 and BG3 alike; any other mode waves
/// BG3 alone (Mars Lighthouse's rain ledges).
#define LINE_SCROLL_ALL_LAYERS 0
/// gLineScroll_WaveSine's entries, a whole cycle: a phase is 16.16 entries.
#define LINE_SCROLL_WAVE_ENTRIES 256
/// The sine's units in a pixel: its peak, 4096, is 16 pixels at an amplitude
/// of FIXED_ONE.
#define LINE_SCROLL_WAVE_UNITS 256
/// The wave's offset in pixels at phase, for a 16.16 amplitude.
#define LINE_SCROLL_WAVE(phase, amplitude)                                     \
  (Iwram_MulFX(gLineScroll_WaveSine[FIXEDtoINT(phase) &                        \
                                    (LINE_SCROLL_WAVE_ENTRIES - 1)],           \
               (amplitude)) /                                                  \
   LINE_SCROLL_WAVE_UNITS)

/// VBlank-pass task for the line scroll: restarts DMA 0 each VBlank to copy the
/// current page's BG1 to BG3 scroll offsets into the scroll registers one
/// scanline at a time, but not while Reveal shows or a battle is due.
void LineScroll_ArmDma(void) {
  struct LineScrollWork *work = Ram_HeapSlots->lineScroll;
  u32 *line = (u32 *)work->rows[work->page];
  volatile u32 *dest = (volatile u32 *)&REG_BG1HOFS;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  volatile u32 *target;

  if (event != NULL) {
    if (event->revealActive != 0)
      return;
    if (event->encounter != 0)
      return;
  }
  {
    volatile u16 *dma = REG_DMA0;
    u16 control;

    control = dma[5];
    dma[5] = control & (u16)~DMA_START_TIMING_DRQ_REPEAT;
    dma[5] &= ~DMA_ENABLE;
    target = dest;
    (void)dma[5];
  }
  *dest = *line++;
  *dest = *line++;
  *dest = *line++;
  Dma_Set(
      line, (void *)target,
      (DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_DEST_RELOAD | DMA_32BIT)
              << DMA_CONTROL_SHIFT |
          sizeof(work->rows[0][0]) / 4,
      REG_DMA0);
}

/// Task for the line scroll: fills the hidden page with each scanline's BG1,
/// BG2 and BG3 offsets, the scroll plus a sine wave for x and for y (an axis
/// with no step stays flat; outside LINE_SCROLL_ALL_LAYERS only BG3 waves),
/// its phase following BG1's y, then shows that page.
void LineScroll_BuildPage(void) {
  struct LineScrollWork *work = Ram_HeapSlots->lineScroll;
  u8 mode = work->mode;
  s32 x1 = (s16)gBgScroll[1].x;
  s32 y1 = (s16)gBgScroll[1].y;
  s32 x2 = (s16)gBgScroll[2].x;
  s32 y2 = (s16)gBgScroll[2].y;
  s32 x3 = (s16)gBgScroll[3].x;
  s32 y3 = (s16)gBgScroll[3].y;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  u16 *row;
  s32 step;
  s32 phase;
  FIXED amplitude;
  s32 scanline;
  u16 wave;
  u16 waveX3;
  u16 waveY3;

  if (event != NULL && event->revealActive != 0)
    return;
  if (mode == LINE_SCROLL_ALL_LAYERS) {
    row = &work->rows[work->page ^ 1][0][0].x;
    step = work->stepX;
    phase = (work->frame + (u16)y1) * work->freqX;
    if (step == 0) {
      for (scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
        row[scanline * 6] = x1;
        row[scanline * 6 + 2] = x2;
        row[scanline * 6 + 4] = x3;
      }
    } else {
      u16 bg1 = x1;
      u16 bg2 = x2;

      waveX3 = x3;
      amplitude = work->ampX;
      for (scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
        wave = LINE_SCROLL_WAVE(phase, amplitude);
        row[0] = bg1 + wave;
        row[2] = bg2 + wave;
        row[4] = waveX3 + wave;
        row += 6;
        phase += step;
      }
    }
    row = &work->rows[work->page ^ 1][0][0].y;
    step = work->stepY;
    phase = (work->frame + (u16)y1) * work->freqY;
    if (step == 0) {
      for (scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
        row[scanline * 6] = y1;
        row[scanline * 6 + 2] = y2;
        row[scanline * 6 + 4] = y3;
      }
    } else {
      u16 bg1 = y1;
      u16 bg2 = y2;
      waveY3 = y3;
      amplitude = work->ampY;
      for (scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
        wave = LINE_SCROLL_WAVE(phase, amplitude);
        *row = bg1 + wave;
        row += 2;
        *row = bg2 + wave;
        row += 2;
        *row = waveY3 + wave;
        row += 2;
        phase += step;
      }
    }
  } else {
    row = &work->rows[work->page ^ 1][0][0].x;
    step = work->stepX;
    phase = (work->frame + (u16)y1) * work->freqX;
    if (step == 0) {
      for (scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
        row[scanline * 6] = x1;
        row[scanline * 6 + 2] = x2;
        row[scanline * 6 + 4] = x3;
      }
    } else {
      waveX3 = x3;
      amplitude = work->ampX;
      for (scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
        wave = LINE_SCROLL_WAVE(phase, amplitude);
        row[0] = x1;
        row[2] = x2;
        row[4] = waveX3 + wave;
        row += 6;
        phase += step;
      }
    }
    row = &work->rows[work->page ^ 1][0][0].y;
    step = work->stepY;
    phase = (work->frame + (u16)y1) * work->freqY;
    if (step == 0) {
      for (scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
        row[scanline * 6] = y1;
        row[scanline * 6 + 2] = y2;
        row[scanline * 6 + 4] = y3;
      }
    } else {
      waveY3 = y3;
      amplitude = work->ampY;
      for (scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
        wave = LINE_SCROLL_WAVE(phase, amplitude);
        row[0] = y1;
        row[2] = y2;
        row[4] = waveY3 + wave;
        row += 6;
        phase += step;
      }
    }
  }
  work->frame++;
  work->page ^= 1;
}

/// Waits for a DMA channel to finish.
static inline void WaitDma(volatile struct DmaRegisters *channel) {
  while (channel->control & DMA_ENABLE << DMA_CONTROL_SHIFT) {
  }
}

/// Starts the line scroll: its work (heap slot HEAP_SLOT_LINE_SCROLL) holds the
/// mode and, for x and y, how fast the wave runs from frame to frame, its step
/// from one scanline to the next and its amplitude; two tasks, one in the
/// vertical blank pass and one in the frame pass, build and send each frame's
/// line offsets.
void LineScroll_InitWave(s32 mode, s32 freqX, s32 stepX, s32 ampX, s32 freqY,
                         s32 stepY, s32 ampY) {
  struct LineScrollWork *work;

  work =
      HeapSlot_AllocEwram(HEAP_SLOT_LINE_SCROLL, sizeof(struct LineScrollWork));
  Dma_Fill32(0, work, sizeof(struct LineScrollWork));
  WaitDma((volatile struct DmaRegisters *)&REG_DMA3SAD);
  work->mode = mode;
  work->freqX = freqX;
  work->freqY = freqY;
  work->ampX = ampX;
  work->ampY = ampY;
  work->stepX = stepX;
  work->stepY = stepY;
  Task_Add(LineScroll_BuildPage, TASK_PASS_FRAME);
  Task_Add(LineScroll_ArmDma, TASK_PASS_VBLANK);
}

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

#undef LINE_SCROLL_ALL_LAYERS
#undef LINE_SCROLL_WAVE_ENTRIES
#undef LINE_SCROLL_WAVE_UNITS
#undef LINE_SCROLL_WAVE
