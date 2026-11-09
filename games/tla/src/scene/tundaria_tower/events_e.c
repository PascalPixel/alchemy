#include "tundaria_tower.h"
#include "call.h"

void Func_02001868(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;


  if (work->actionCounter != 0) {
    Call1((void (*)())GameFlag_SetBit, 0x106);
    Func_02003100();
    Func_02003140();
    GameFlag_ClearBit(0x106);
  }
}
