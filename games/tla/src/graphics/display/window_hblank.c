#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "display_transition.h"

// Each frame, before drawing: turns windows 0 and 1 on, writes the first four
// values of the shown page into WININ, WINOUT, WIN0H and WIN1H, and has DMA 0
// copy the rest into WIN0H and WIN1H one word each HBlank, as ☀️'s
// BattleFx_StartWindowHBlankDma does.
void BattleFx_StartWindowHBlankDma(void) {
  struct DisplayTransitionState *work;
  u16 *src;
  volatile u16 *channel;
  s32 in;
  s32 out;
  s32 value;
  s32 other;
  s32 dispHigh;

  /* FAKEMATCH(one-pass): the transition work loaded before the page offset
   * is started; as a plain assignment, 2 of 68 instructions differ in
   * tla-en. */
  do {
    work = Ram_HeapSlots->displayTransition;
  } while (0);
  src = work->pages[work->page];
  channel = REG_DMA0;
  // Stop the previous transfer before priming the first line's registers.
  {
    u16 control;

    /* FAKEMATCH(temporary): REG_DISPCNT's high bits (128, shifted by 19
     * below) started in a variable of their own, after the page load as the
     * native does; as `REG_DISPCNT |= ...`, 2 of 68 instructions differ in
     * tla-en. */
    dispHigh = 128;
    /* FAKEMATCH(barrier): the asm on dispHigh raises its start to the slot
     * after the page load; without it, 2 of 68 instructions differ in
     * tla-en. */
    __asm__("" : "+r"(dispHigh));
    /* FAKEMATCH(temporary): DMA 0's control read into its own variable, so
     * the barrier below can stand between the read and the mask; as
     * `channel[5] &= ~DMA_START_TIMING_DRQ_REPEAT`, 2 of 68 instructions
     * differ in tla-en. */
    control = channel[5];
    /* FAKEMATCH(barrier): the control read before the first mask is
     * started, as the native does; without the barrier, 2 of 68
     * instructions differ in tla-en. */
    __asm__ volatile("");
    channel[5] = control & ~DMA_START_TIMING_DRQ_REPEAT;
  }
  channel[5] &= ~DMA_ENABLE;
  (void)channel[5];
  // REG_DISPCNT
  dispHigh <<= 19;
  *(volatile u16 *)dispHigh |= DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
  /* FAKEMATCH(one-pass): the value loaded before the register address is
   * started; as a plain assignment, 2 of 68 instructions differ in
   * tla-en. */
  do {
    in = *src;
  } while (0);
  REG_WININ = in;
  src++;
  out = *src;
  REG_WINOUT = out;
  src++;
  /* FAKEMATCH(one-pass): the value loaded before the register address is
   * started; as a plain assignment, 2 of 68 instructions differ in
   * tla-en. */
  do {
    value = *src;
  } while (0);
  REG_WIN0H = value;
  src++;
  /* FAKEMATCH(one-pass): the value loaded before the register address is
   * started; as a plain assignment, 2 of 68 instructions differ in
   * tla-en. */
  do {
    other = *src;
  } while (0);
  REG_WIN1H = other;
  src++;
  /* FAKEMATCH(temporary): both vertical ranges (0 to 160) stored from the
   * register that held the last value; as WIN_RANGE(0, 160) at each store,
   * 7 of 68 instructions differ in tla-en. */
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
