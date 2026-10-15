#include "types.h"
#include "ram_buffer.h"

struct SparkWork {
    u8 unk0000[0x1604];
    s16 resource;
};

void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Resource_ResetEntry(s32 entry);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_02004114(void);

void Func_02004310(void)
{
    struct SparkWork *work = Ram_HeapSlots->menuRuntime;

    Scheduler_RemoveCallbackFar(Func_02004114);
    Resource_ResetEntry(work->resource);
    Runtime_ReleaseHeapBlock(220);
}
