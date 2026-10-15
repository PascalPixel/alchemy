#include "work9c.h"
#include "mem.h"
#include "resource.h"
#include "callback_scheduler.h"
#include "dma.h"

extern const u8 gData_080f0c04[];

void Func_080d8a00(void);
void Func_080d8c10(void);

void Func_080d897c(void)
{
    struct Work9c *work = Runtime_AllocateBlock(156, sizeof(struct Work9c));
    void *buffer = MemAlloc(0x100);

    Dma_Fill32(0, work, sizeof(struct Work9c));
    work->vramEntry = Resource_FindFreeEntry();
    Resource_DecodeType01(gData_080f0c04, buffer);
    VramBlock_LoadCached(work->vramEntry, 0x100, buffer);
    Scheduler_AddOrUpdateCallback(Func_080d8a00, 0x480);
    Scheduler_AddOrUpdateCallback(Func_080d8c10, 0x48f);
    MemFree(buffer);
}
