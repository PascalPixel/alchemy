#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "bg_scroll.h"
#include "display_scroll.h"

extern u32 Data_02007518;
extern volatile s32 gData_0200751c;
extern s16 gData_02007520;
extern s16 Data_02007516;
extern s16 Data_02007514;
extern s16 Data_02007524;
extern u8 *Data_02007528;

#define ABS(v) ((v) < 0 ? -(v) : (v))

void Func_081a133c(void) {
  /* FAKEMATCH(volatile): the scroll word declared volatile, read at every use
   * as the ROM reads it; as a plain s32, 70 of 212 instructions differ in
   * tla-en. */
  s32 old = gData_0200751c;
  s32 d;
  s32 scroll;
  u8 *source;
  u8 *destination;
  u32 start;
  u32 from;
  u32 offset;
  s32 i;

  if (Data_02007516 < 64) {
    s32 t = (s16)++Data_02007516 / 4;
    s32 alpha;

    if (Data_02007514 & 1) {
      alpha = ((16 - t) << 8) | t;
      REG_BLDALPHA = alpha;
    } else {
      alpha = (t << 8) | (16 - t);
      REG_BLDALPHA = alpha;
    }
  }
  if (gScrollTarget != gData_02007520) {
    d = gScrollTarget - gData_02007520;
    if (ABS(d) > Data_02007524) {
      if (d > 0)
        d = Data_02007524;
      else
        d = -Data_02007524;
    }
    gData_02007520 += d;
  }
  {
    /* FAKEMATCH(temporary): the form kept; as scroll = gData_0200751c, 6 of 212
     * instructions differ in tla-en. */
    s32 *word = &gData_0200751c;
    s32 target = gData_02007520 << 16;

    scroll = *word;
    if (scroll != target) {
      d = target - scroll;
      if (ABS(d) > Data_02007524 << 14)
        d /= 8;
      if (ABS(d) > Data_02007524 << 16) {
        if (d > 0)
          d = Data_02007524 << 16;
        else
          d = -(Data_02007524 << 16);
      }
      gData_0200751c += d;
      Data_02007518 += d;
    }
  }
  if ((gData_0200751c ^ old) & 0x80000) {
    source = Data_02007528;
    destination = (u8 *)0x06000000;
    start = (Data_02007518 >> 19) << 6;
    if (Data_02007514 != 0)
      destination = (u8 *)0x06008000;
    scroll = gData_0200751c;
    if (scroll > old) {
      from = (((u32)scroll >> 19) << 6) + 0x780;
      offset = (start + 0x780) & 0x7ff;
      for (i = 0; i < 15; i++) {
        Dma_Set(source + from, destination + offset, 0x84000010, REG_DMA3);
        source += 0x1000;
        destination += 0x800;
      }
    } else {
      from = ((u32)gData_0200751c >> 19) << 6;
      offset = start & 0x7ff;
      for (i = 0; i < 15; i++) {
        Dma_Set(source + from, destination + offset, 0x84000010, REG_DMA3);
        source += 0x1000;
        destination += 0x800;
      }
    }
  }
  d = Data_02007518 >> 16;
  gBgScroll[2].x = d;
  gBgScroll[3].x = d;
}
