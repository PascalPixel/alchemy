#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "battle_fx_work.h"

// The scenes' frame callbacks that copy the display registers the effect
// keeps in its work to the hardware.

/// Writes the window, display and blend registers, all at once.
void Graphics_ApplyWindowBlendRegisters(void) {
  /* FAKEMATCH(pin): the work pointer kept in r2, so the registers' address
   * takes r1; as a plain local, 31 of 55 instructions differ in tla-en. */
  register struct BattleFxWork *work asm("r2") =
      (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];

  {
    u32 value = work->win0h;

    /* FAKEMATCH(barrier): an empty asm through the first value loads it before
     * the register's address is built; without it, 14 of 55 instructions
     * differ in tla-en. */
    __asm__ volatile("" : "+l"(value));
    REG_WIN0H = value;
  }
  REG_WIN0V = work->win0v;
  REG_WIN1H = work->win1h;
  REG_WIN1V = work->win1v;
  REG_WININ = work->winin;
  REG_WINOUT = work->winout;
  REG_DISPCNT = work->dispcnt;
  REG_BLDCNT = work->bldcnt;
  REG_BLDALPHA = work->bldalpha;
}

/// Writes background 2's scale (BG2PA and BG2PD) and its reference point.
void Display_ApplyBg2Affine(void) {
  /* FAKEMATCH(pin): the work pointer kept in r2, so the registers' address
   * takes r1; as a plain local, 16 of 30 instructions differ in tla-en. */
  register struct BattleFxWork *work asm("r2") =
      (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];

  {
    u32 value = work->bg2pa;

    /* FAKEMATCH(barrier): an empty asm through the first value loads it before
     * the register's address is built; without it, 14 of 30 instructions
     * differ in tla-en. */
    __asm__ volatile("" : "+l"(value));
    REG_BG2PA = value;
  }
  REG_BG2PD = work->bg2pd;
  /* FAKEMATCH(barrier): an empty asm keeps the next offset's step after the
   * store before it; without it, 2 of 30 instructions differ in tla-en. */
  __asm__ volatile("");
  REG_BG2X = work->bg2x;
  REG_BG2Y = work->bg2y;
}

/// Writes background 2's reference point.
void Display_ApplyBg2Reference(void) {
  /* FAKEMATCH(pin): the work pointer kept in r2, so the registers' address
   * takes r1; as a plain local, 20 of 20 instructions differ in tla-en. */
  register struct BattleFxWork *work asm("r2") =
      (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];
  u32 hold;

  /* FAKEMATCH(barrier): an empty asm sets hold, which keeps r1 until the
   * first value is read, so the offset takes r0; without hold and its asm,
   * 8 of 20 instructions differ in tla-en. */
  __asm__ volatile("" : "=l"(hold));
  {
    u32 value = work->bg2x;

    /* FAKEMATCH(barrier): an empty asm through the first value loads it before
     * the register's address is built, and ends hold; as an asm on hold
     * alone, 14 of 20 instructions differ in tla-en. */
    __asm__ volatile("" : "+l"(value) : "l"(hold));
    REG_BG2X = value;
  }
  REG_BG2Y = work->bg2y;
}
