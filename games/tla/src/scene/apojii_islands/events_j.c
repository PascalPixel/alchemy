#include "local.h"

void Func_02001ba0(s32 id) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 4 of 20 instructions
   * differ in tla-en. */
  u8 *work = Ram_HeapSlots->unk70[1];
  struct Actor *object = Actor_GetById(id);
  s32 *slot = (s32 *)(work + SPLIT_CONSTANT(0x800, 8));

  *slot = 0;
  if (id != 0 && object != 0)
    *slot = (s32)&object->x;
}
