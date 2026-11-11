#include "local.h"

s32 Func_02003ba4(s32 index) {
  u8 *base = (u8 *)VENT_WORK;

  /* FAKEMATCH(barrier): the empty asm loads the work address before the index
   * is scaled, as the ROM does; as no asm. */
  asm volatile("");
  return ((struct VentRow *)(base + index * 20))->active;
}
