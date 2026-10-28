#include "tundaria_tower.h"

void Func_02002ebc(void) {
  struct TundariaTowerSparkWork *work = Ram_HeapSlots->menuRuntime;

  Task_RemoveFar(Func_02002cc0);
  Resource_ResetEntry(work->resource);
  Runtime_ReleaseHeapBlock(220);
}
