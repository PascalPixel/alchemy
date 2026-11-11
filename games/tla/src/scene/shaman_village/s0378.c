#include "shaman_village.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "event_inline.h"

extern s16 gData_02006350[];

u32 Random16Far(void);

static inline s32 LowestLineOffset(void) {
  /* FAKEMATCH(wrapper): the bound returned by an inline, built inside the loop
   * as the listing builds it; as the constant -64, loop.c hoists it into r8
   * (differ 21). */
  return -64;
}

/// Fills BG0's line scroll table with a jittered sine wave and streams it
/// to BG0HOFS by HBlank DMA 0.
void Func_02000378(void) {
  s16 *line = gData_02006350;
  s32 i;
  s16 *source;
  volatile u16 *scroll;
  volatile u16 *dma;

  for (i = 0; i != 160; i++, line++) {
    s32 range = (160 - i) >> 2;

    *line = (Iwram_MulQ16(Math_Sine((i + *(u8 *)&gFrameCount) << 8),
                          (144 - i) << 10) >>
             11) -
            4;
    *line += Random16Far() % (range * 2) - range;
    if (*line > 56)
      *line -= 56;
    else if (*line < LowestLineOffset())
      *line += 64;
  }
  line = gData_02006350;
  scroll = (volatile u16 *)REG_BG0HOFS;
  *scroll = *line;
  dma = REG_DMA0;
  dma[5] &= 0xc5ff; // stop the start timing and the repeat
  dma[5] &= 0x7fff; // stop the channel
  source = line + 1;
  (void)dma[5];
  /* FAKEMATCH(temporary): line, reused for the destination, keeps the source
   * step before the last control read; as Dma_Set(source, scroll, ...),
   * combine folds the step into Dma_Set's r0 copy after the read (differ 2),
   * and an asm barrier on the source also matches. */
  line = (s16 *)scroll;
  Dma_Set(source, line, 0xa2600001, (volatile u32 *)dma);
}
