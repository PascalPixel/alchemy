#include "local.h"

void Func_02000cfc(void) {
  s32 b = 8;
  s32 c = 9;

  /* FAKEMATCH(barrier): the empty asm on the two coordinates, so they load
   * before the first argument as the native has; as Func_02002178(1, 8, 9),
   * 2 of 10 instructions differ in tla-en (the first argument loads first).
   */
  __asm__ volatile("" : "+r"(b), "+r"(c));
  Func_02002178(1, b, c);
  Event_SetFlag(0x203);
}
