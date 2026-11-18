/// @file
/// Fills BG0's line scroll table with a jittered sine wave and streams it
/// to BG0HOFS by HBlank DMA 0. Each overlay that has it includes this body
/// after naming the function (WAVE_SCROLL) and its table of a scroll for each
/// screen line (WAVE_SCROLL_LINES).
#include "types.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/frame.h"

extern s16 WAVE_SCROLL_LINES[];

/// A line's swing on the sine wave falls by half a pixel a line down the
/// screen, to nothing at WAVE_SCROLL_CALM_LINE.
#define WAVE_SCROLL_CALM_LINE 144
/// A line scrolled past these is pulled back by as much.
#define WAVE_SCROLL_MAX 56
#define WAVE_SCROLL_MIN (-64)

void WAVE_SCROLL(void) {
  s16 *line = WAVE_SCROLL_LINES;
  s32 i;
  s16 *source;
  volatile u16 *scroll;
  volatile u16 *dma;

  for (i = 0; i != DISPLAY_HEIGHT; i++, line++) {
    s32 range = (DISPLAY_HEIGHT - i) >> 2;

    // A sine of the line and the frame, (WAVE_SCROLL_CALM_LINE - i) / 2 pixels
    // either way, less 4, jittered by up to a quarter of the lines below.
    *line = (Iwram_MulFX(Trig_Sin((i + (u8)gFrameCount) * (TRIG_TURN / 256)),
                         (WAVE_SCROLL_CALM_LINE - i) * (FIXED_ONE / 64)) >>
             11) -
            4;
    *line += Random16() % (range * 2) - range;
    if (*line > WAVE_SCROLL_MAX)
      *line -= WAVE_SCROLL_MAX;
    else if (*line < WAVE_SCROLL_MIN)
      *line -= WAVE_SCROLL_MIN;
  }
  line = WAVE_SCROLL_LINES;
  scroll = (volatile u16 *)REG_BG0HOFS;
  *scroll = *line;
  dma = REG_DMA0;
  dma[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
  dma[5] &= ~DMA_ENABLE;
  source = line + 1;
  (void)dma[5];
  Dma_Set(source, (void *)scroll, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)dma);
}

#undef WAVE_SCROLL_CALM_LINE
#undef WAVE_SCROLL_MAX
#undef WAVE_SCROLL_MIN
