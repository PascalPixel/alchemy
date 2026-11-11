#include "local.h"

void Func_02003c00(s32 index, s32 value) {
  u8 *base = (u8 *)VENT_WORK;

  /* FAKEMATCH(barrier): the empty asm loads the work address before the index
   * is scaled, as the ROM does; as no asm. */
  asm volatile("");
  ((struct VentRow *)(base + index * 20))->value = value;
}
