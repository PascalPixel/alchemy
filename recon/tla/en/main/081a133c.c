// Near miss, differ 2 of 212 (tla-en, 2026-10-30): scheduler order only. The
// ROM issues the column test's pointer copy (adds r0, r4) between the scroll
// word's store and Data_02007518's load; the draft issues the load first.
// That load opens a read-modify-write, which -mthumb-slow-loads lets issue
// straight after a store (alchemy-gcc's README: a measured exception, and this
// is a case it misses), so no source form reaches the ROM's order.
// 2026-10-30: writing the update as gData_0200751c += d, not through the block
// pointer, turns PRE's reload of the column test's address into the copy the
// ROM makes, as the position's already was; and the backward loop is the
// forward loop's for (i = 0; i < 15; i++), which loop.c reverses, emitting
// its 14 after the two hoisted step constants (from 4).
// 2026-10-25, with the Makefile's options (-mthumb-slow-loads): reading the
// column test through the block pointer, kept in scope or copied to a local,
// gives 56. A greedy device search found nothing lower.
// The scrolling picture scene's per-frame callback
// (Func_081a150c adds it). Fades BLDALPHA in over 64 frames, eases the
// position toward the target at most the speed a frame, eases the 16.16
// scroll toward the position, and when the scroll crosses an 8-pixel column
// copies the newly visible column of 15 rows into the wrapping tilemap, as
// Func_081a1294 does for the whole map; then writes the scroll to BG2 and BG3.
// The native reads the scroll word at every use (volatile here; plain, differ
// 80) except where one read serves a compare and its arm (the locals). A
// block pointer to the scroll word loads its address before the position's
// ldrsh (at function scope for the later reads too, differ 32). A pin and
// barrier for the address moved the allocation (differ 156); five permuter
// minutes found nothing.
// 2026-10-24: the later reads through a second pointer copied from the
// block's (32). Func_081a1294, which this callback's column copy mirrors, is
// adopted (games/tla/src/menu/credits/scroll_map.c).
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
    /* FAKEMATCH(temporary): the form kept; as scroll = gData_0200751c, 6 of 212 instructions differ in tla-en. */
    volatile s32 *word = &gData_0200751c;
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
