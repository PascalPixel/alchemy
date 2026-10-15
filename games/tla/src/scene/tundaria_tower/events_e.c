#include "tundaria_tower.h"

void Func_02001868(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  /* FAKEMATCH: a one-pass block loads the event work before its offset is built
   * (one reordered row). */
  do {
  } while (0);
  if (work->actionCounter != 0) {
    Call1((void (*)())GameFlag_SetBit, 0x106);
    Func_02003100();
    Func_02003140();
    Call1((void (*)())GameFlag_ClearBit, 0x106);
  }
}
