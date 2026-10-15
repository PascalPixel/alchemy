#include "types.h"
#include "ram_buffer.h"
#include "resource.h"
#include "mem.h"

struct EasternAlhafraSparkWork {
    u8 unknown_0000[0x1604];
    s16 resource;
};

void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Func_02003ec4(void);

void Func_020040c0(void)
{
    struct EasternAlhafraSparkWork *work = Ram_HeapSlots->menu_runtime;

    Scheduler_RemoveCallbackFar(Func_02003ec4);
    Resource_ResetEntry(work->resource);
    Runtime_ReleaseHeapBlock(220);
}
