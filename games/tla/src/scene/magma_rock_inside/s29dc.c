#include "local.h"
#include "call.h"

void Func_02004da0(s32 id, s32 x, s32 z);

void Func_020029dc(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 4 of 25 instructions
   * differ in tla-en. */
  if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 0xd30) == -1) {
    if (!GameFlag_Test(SPLIT_CONSTANT(0x300, 5)))
      Func_02004da0(11, 0, 0);
  }
}
