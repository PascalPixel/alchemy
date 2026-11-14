/// @file
/// The line scroll: per-line scroll offsets fed by HBlank DMA.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"
#include "fixed_math.h"
#include "flags.h"
#include "sounds.h"
#include "module/field/field.h"

/// Writes the current page's first entry each frame; HBlank DMA writes the
/// rest.
void LineScroll_ArmDma(void) {
  struct DmaFxPages *work = Ram_HeapSlots->unk84[1];
  u32 *line = work->pages[work->page].lines;
  volatile u32 *dest = (volatile u32 *)&REG_BG1HOFS;
  u8 *event = Ram_HeapSlots->fieldWork;
  volatile u32 *target;

  if (event != NULL) {
    if (*(s8 *)(event + 0xca4) != 0)
      return;
    if (*(s16 *)(event + 0x164) != 0)
      return;
  }
  {
    volatile u16 *dma = REG_DMA0;
    u16 control;

    control = dma[5];
    dma[5] = control & 0xc5ff; // Clear start timing and repeat.
    dma[5] &= 0x7fff;          // Stop.
    target = dest;
    (void)dma[5];
  }
  *dest = *line++;
  *dest = *line++;
  *dest = *line++;
  Dma_Set(
      line, (void *)target,
      (DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_DEST_RELOAD | DMA_32BIT)
              << 16 |
          3,
      REG_DMA0);
}

void LineScroll_BuildPage(void) {
  struct DisplayScrollWork *work = Ram_HeapSlots->unk84[1];
  u8 mode = work->mode;
  s32 x1 = (s16)gBgScroll[1].x;
  s32 y1 = (s16)gBgScroll[1].y;
  s32 x2 = (s16)gBgScroll[2].x;
  s32 y2 = (s16)gBgScroll[2].y;
  s32 x3 = (s16)gBgScroll[3].x;
  s32 y3 = (s16)gBgScroll[3].y;
  u8 *event = Ram_HeapSlots->fieldWork;
  u16 *row;
  s32 step;
  s32 phase;
  s32 amplitude;
  s32 scanline;
  u16 wave;
  u16 waveX3;
  u16 waveY3;

  if (event != NULL && *(s8 *)(event + 0xca4) != 0)
    return;
  if (mode == 0) {
    row = &work->rows[work->page ^ 1][0][0].x;
    step = work->stepX;
    phase = (work->frame + (u16)y1) * work->freqX;
    if (step == 0) {
      for (scanline = 0; scanline != 160; scanline++) {
        row[scanline * 6] = x1;
        row[scanline * 6 + 2] = x2;
        row[scanline * 6 + 4] = x3;
      }
    } else {
      u16 a = x1;
      u16 b = x2;

      waveX3 = x3;
      amplitude = work->ampX;
      for (scanline = 0; scanline != 160; scanline++) {
        wave =
            Iwram_MulQ16(gLineScroll_WaveSine[(phase >> 16) & 255], amplitude) /
            256;
        row[0] = a + wave;
        row[2] = b + wave;
        row[4] = waveX3 + wave;
        row += 6;
        phase += step;
      }
    }
    row = &work->rows[work->page ^ 1][0][0].y;
    step = work->stepY;
    phase = (work->frame + (u16)y1) * work->freqY;
    if (step == 0) {
      for (scanline = 0; scanline != 160; scanline++) {
        row[scanline * 6] = y1;
        row[scanline * 6 + 2] = y2;
        row[scanline * 6 + 4] = y3;
      }
    } else {
      u16 a = y1;
      u16 b = y2;
      waveY3 = y3;
      amplitude = work->ampY;
      for (scanline = 0; scanline != 160; scanline++) {
        wave =
            Iwram_MulQ16(gLineScroll_WaveSine[(phase >> 16) & 255], amplitude) /
            256;
        *row = a + wave;
        row += 2;
        *row = b + wave;
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
      for (scanline = 0; scanline != 160; scanline++) {
        row[scanline * 6] = x1;
        row[scanline * 6 + 2] = x2;
        row[scanline * 6 + 4] = x3;
      }
    } else {
      waveX3 = x3;
      amplitude = work->ampX;
      for (scanline = 0; scanline != 160; scanline++) {
        wave =
            Iwram_MulQ16(gLineScroll_WaveSine[(phase >> 16) & 255], amplitude) /
            256;
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
      for (scanline = 0; scanline != 160; scanline++) {
        row[scanline * 6] = y1;
        row[scanline * 6 + 2] = y2;
        row[scanline * 6 + 4] = y3;
      }
    } else {
      waveY3 = y3;
      amplitude = work->ampY;
      for (scanline = 0; scanline != 160; scanline++) {
        wave =
            Iwram_MulQ16(gLineScroll_WaveSine[(phase >> 16) & 255], amplitude) /
            256;
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

static __inline__ void WaitDma(struct DmaRegisters volatile *channel) {
  while ((channel->control & DMA_ENABLE << 16) != 0) {
  }
}

/// Starts the line scroll: its work (heap slot 136) holds the mode and, for x
/// and y, the wave's frequency, its step each frame and its amplitude; two
/// tasks, one in the vertical blank pass and one in the frame pass, build and
/// send each frame's line offsets.
void LineScroll_InitWave(s32 mode, s32 freqX, s32 stepX, s32 ampX, s32 freqY,
                         s32 stepY, s32 ampY) {
  struct DmaFxWork *work = HeapSlot_AllocEwram(136, sizeof(struct DmaFxWork));
  volatile u32 zero;

  zero = 0;
  Dma_Set((void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (sizeof(*work) / 4),
          REG_DMA3);
  WaitDma((volatile struct DmaRegisters *)&REG_DMA3SAD);
  work->kind = mode;
  work->unkf08 = freqX;
  work->unkf0c = freqY;
  work->unkf18 = ampX;
  work->unkf1c = ampY;
  work->unkf10 = stepX;
  work->unkf14 = stepY;
  Task_Add(LineScroll_BuildPage, TASK_PASS_VBLANK);
  Task_Add(LineScroll_ArmDma, TASK_PASS_FRAME);
}

/// Stops the line scroll: removes its two tasks and stops DMA 0.
void LineScroll_Stop(void) {
  Task_Remove(LineScroll_ArmDma);
  Task_Remove(LineScroll_BuildPage);
  {
    volatile u16 *dma = REG_DMA0;

    u16 control;

    control = dma[5];
    dma[5] = control & 0xc5ff; // Clear start timing and repeat.
    dma[5] &= 0x7fff;          // Stop.
    (void)dma[5];
  }
}
