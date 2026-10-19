#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "battle_fx_work.h"

// The scenes' frame callbacks that copy the display registers the effect
// keeps in its work to the hardware. The native reads each value before it
// starts the register's address and keeps every step in source order; agscc
// starts the address first, the pool and load ordering that recurs through
// ⚓️. The devices below hold the first value behind a barrier and keep the
// later stores in order.

// Writes background 2's scale (BG2PA and BG2PD) and its reference point.
void Display_ApplyBg2Affine(void) {
  /* FAKEMATCH(fixed register): the work pointer kept in r2; as a plain local
   * with the barriers below, 32 of 30 instructions differ in tla-en (8 as
   * plain C). */
  register struct BattleFxWork *work asm("r2") =
      (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];

  /* FAKEMATCH(barrier): the work pointer loads before the first offset
   * starts; as plain C, 2 of 30 instructions differ in tla-en. */
  __asm__ volatile("");
  {
    u32 value = work->bg2pa;

    /* FAKEMATCH(barrier): the first value is read before the register's
     * address starts; as a plain barrier, 14 of 30 instructions differ in
     * tla-en. */
    __asm__ volatile("" : "+l"(value));
    REG_BG2PA = value;
  }
  /* FAKEMATCH(barrier): the offset's step (adds r0, #2) stays after the
   * store before it; as plain C, 2 of 30 instructions differ in tla-en. */
  __asm__ volatile("");
  REG_BG2PD = work->bg2pd;
  /* FAKEMATCH(barrier): the offset's step (subs r0, #10) stays after the
   * store before it; as plain C, 2 of 30 instructions differ in tla-en. */
  __asm__ volatile("");
  REG_BG2X = work->bg2x;
  REG_BG2Y = work->bg2y;
}

// Writes background 2's reference point.
void Display_ApplyBg2Reference(void) {
  /* FAKEMATCH(fixed register): the work pointer kept in r2; as a plain local
   * with the devices below, 26 of 20 instructions differ in tla-en (4 as
   * plain C). */
  register struct BattleFxWork *work asm("r2") =
      (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];
  /* FAKEMATCH(fixed register): r1 held until the first value is read, as the
   * register's address would hold it if started first, so that the offset
   * takes r0; without the hold, 8 of 20 instructions differ in tla-en. */
  register u32 hold asm("r1");

  /* FAKEMATCH(barrier): starts the hold on r1 after the work pointer's load;
   * it goes with the hold above. */
  __asm__ volatile("" : "=l"(hold));
  {
    u32 value = work->bg2x;

    /* FAKEMATCH(barrier): the first value is read before the register's
     * address starts, and the hold ends; without the value, 14 of 20
     * instructions differ in tla-en. */
    __asm__ volatile("" : "+l"(value) : "l"(hold));
    REG_BG2X = value;
  }
  REG_BG2Y = work->bg2y;
}
