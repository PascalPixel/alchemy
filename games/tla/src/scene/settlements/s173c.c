#include "local.h"
#include "event_work.h"

void Func_0200173c(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;


  if (work->actionCounter != 0) {
    Flag_Set(0x106);
    Func_02002118();
    Func_02002148();
    Event_ClearFlag(0x106);
  }
}
