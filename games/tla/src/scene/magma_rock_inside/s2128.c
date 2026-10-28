#include "local.h"

void Func_02004da0(s32 id, s32 x, s32 z);

void Func_02002128(void) {
  u8 *work = (u8 *)Ram_HeapSlots->eventWork;

  /* FAKEMATCH: a one-pass block loads the event work before its offset is built
   * (one reordered row). */
  do {
  } while (0);
  *(work + 0xd38) = 1;
  if (!GameFlag_Test(0x304)) {
    Actor_SetPartAttribute(Actor_GetById(22), 15);
    Func_02004da0(22, PIXELS(632), PIXELS(312));
    Actor_GetById(22)->scaleX = Actor_GetById(22)->scaleY = 0x1000;
  }
}
