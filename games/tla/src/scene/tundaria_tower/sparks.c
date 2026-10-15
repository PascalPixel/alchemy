#include "tundaria_tower.h"

void Func_02002ebc(void) {
  struct TundariaTowerSparkWork *work = Ram_HeapSlots->menuRuntime;

  Scheduler_RemoveCallbackFar(Func_02002cc0);
  Resource_ResetEntry(work->resource);
  Runtime_ReleaseHeapBlock(220);
}
