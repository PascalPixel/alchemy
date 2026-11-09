#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "display_transition.h"

/// Each frame, before drawing: turns windows 0 and 1 on, writes the first four
/// values of the shown page into WININ, WINOUT, WIN0H and WIN1H, and has DMA 0
/// copy the rest into WIN0H and WIN1H one word each HBlank.
// As ☀️'s FieldEffect_StartWindowHBlankDma does.
void BattleEffect_StartWindowHBlankDma(void) {
  struct ScreenTransitionState *work;
  u16 *src;
  volatile u16 *channel;
  s32 in;
  s32 out;
  s32 other;
  s32 dispHigh;

  work = Ram_HeapSlots->screenTransition;
  src = work->pages[work->page];
  channel = REG_DMA0;
  // Stop the previous transfer before priming the first line's registers.
  {
    u16 control;

    /* FAKEMATCH(temporary): REG_DISPCNT's high bits start in a variable of
     * their own, after the page load as the native does; as
     * `REG_DISPCNT |= ...`, 2 of 68 instructions differ in tla-en. */
    dispHigh = 128;
    /* FAKEMATCH(temporary): DMA 0's control read into its own variable for
     * the barrier below; as `channel[5] &= ~DMA_START_TIMING_DRQ_REPEAT`, 2
     * of 68 instructions differ in tla-en. */
    control = channel[5];
    channel[5] = control & ~DMA_START_TIMING_DRQ_REPEAT;
  }
  channel[5] &= ~DMA_ENABLE;
  (void)channel[5];
  // REG_DISPCNT
  dispHigh <<= 19;
  *(volatile u16 *)dispHigh |= DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
  // The load in a once-only block comes before the register address is
  // built; as the plain load, 2 of 68 instructions differ in tla-en.
  do {
    in = *src;
  } while (0);
  REG_WININ = in;
  src++;
  out = *src;
  REG_WINOUT = out;
  src++;
  REG_WIN0H = *src;
  src++;
  /* FAKEMATCH(temporary): WIN1H's value read into other before the register
   * address is built; as REG_WIN1H = *src, 8 of 68 instructions differ in
   * tla-en. */
  other = *src;
  REG_WIN1H = other;
  src++;
  // other is reused for both vertical ranges; WIN_RANGE(0, 160) at each
  // store differs.
  other = WIN_RANGE(0, 160);
  REG_WIN0V = other;
  REG_WIN1V = other;
  // HBlank repeats one word: both horizontal window bounds for the next line.
  Dma_Set(
      src, (void *)&REG_WIN0H,
      (DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_32BIT | DMA_DEST_RELOAD)
              << 16 |
          1,
      (volatile u32 *)channel);
}
