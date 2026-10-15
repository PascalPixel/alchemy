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
    Object_SetPartAttribute(Object_GetById(22), 15);
    Func_02004da0(22, 632 << 16, 312 << 16);
    Object_GetById(22)->scaleX = Object_GetById(22)->scaleY = 0x1000;
  }
}
