#include "types.h"
#include "event_inline.h"

void Func_020001a0(s32 a, s32 b, s32 c);

void Func_02000054(void) {
  s32 a1 = 13;
  s32 a2 = 14;

  /* FAKEMATCH(barrier): the asm on the later arguments loads them before the
   * first, as the native does, where without it the first loads earlier; as
   * Func_020001a0(1, 13, 14), 2 of 10 instructions differ in tla-en. */
  asm volatile("" : "+l"(a1), "+l"(a2));
  Func_020001a0(1, a1, a2);
  GameFlag_SetBit(0x203);
}
