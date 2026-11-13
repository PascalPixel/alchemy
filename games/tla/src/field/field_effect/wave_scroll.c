/// @file
/// Fills BG0's line scroll table with a jittered sine wave and streams it
/// to BG0HOFS by HBlank DMA 0. Each overlay that has it includes this body
/// after naming the function (WAVE_SCROLL) and its 160-line table
/// (WAVE_SCROLL_LINES).
#include "types.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/frame.h"

extern s16 WAVE_SCROLL_LINES[];

void WAVE_SCROLL(void) {
  s16 *line = WAVE_SCROLL_LINES;
  s32 i;
  s16 *source;
  volatile u16 *scroll;
  volatile u16 *dma;

  for (i = 0; i != 160; i++, line++) {
    s32 range = (160 - i) >> 2;

    *line = (Iwram_MulQ16(Trig_Sin((i + *(u8 *)&gFrameCount) << 8),
                          (144 - i) << 10) >>
             11) -
            4;
    *line += Random16() % (range * 2) - range;
    if (*line > 56)
      *line -= 56;
    else if (*line < -64)
      *line += 64;
  }
  line = WAVE_SCROLL_LINES;
  scroll = (volatile u16 *)0x04000010;
  *scroll = *line;
  dma = REG_DMA0;
  dma[5] &= 0xc5ff; // stop the start timing and the repeat
  dma[5] &= 0x7fff; // stop the channel
  source = line + 1;
  (void)dma[5];
  Dma_Set(source, (void *)scroll, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)dma);
}
