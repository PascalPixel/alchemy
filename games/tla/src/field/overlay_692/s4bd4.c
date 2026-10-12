#include "local.h"

struct Ov692SparkWork {
    u8 unknown_0000[0x1604];
    s16 resource;
};

void Resource_ResetEntry(s32 entry);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_020049d8(void);

void Func_02004bd4(void)
{
    struct Ov692SparkWork *work = Ram_HeapSlots->menu_runtime;

    Scheduler_RemoveCallbackFar(Func_020049d8);
    Resource_ResetEntry(work->resource);
    Runtime_ReleaseHeapBlock(220);
}
