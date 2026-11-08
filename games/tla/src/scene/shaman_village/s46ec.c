#include "shaman_village.h"
#include "call.h"

void Func_020046bc(void);

void Func_020046ec(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 0 of 29 instructions
   * differ in tla-en. */
  struct ShamanVillageEventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 521;
  if (GameFlag_Test(SPLIT_CONSTANT(0x9fe))) {
    Task_Add(Func_020046bc, 0x480);
  } else {
    Func_02004934(9, 0, 0);
    Func_02004934(10, 0, 0);
  }
}
