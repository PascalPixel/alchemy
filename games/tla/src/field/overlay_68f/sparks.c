#include "ov68f.h"

void Func_02002ebc(void)
{
    struct Ov68fSparkWork *work = Ram_HeapSlots->menu_runtime;

    Scheduler_RemoveCallbackFar(Func_02002cc0);
    Resource_ResetEntry(work->resource);
    Runtime_ReleaseHeapBlock(220);
}
