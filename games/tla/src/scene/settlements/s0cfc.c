#include "local.h"

void Func_02000cfc(void) {
  s32 b = 8;
  s32 c = 9;

  /* FAKEMATCH: the native loads the two coordinates before the first argument;
   * a direct call and a forwarding inline load it first. */
  __asm__ volatile("" : "+r"(b), "+r"(c));
  Func_02002178(1, b, c);
  Event_SetFlag(0x203);
}
