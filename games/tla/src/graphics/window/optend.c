#include "types.h"
#include "mem.h"
#include "callback_scheduler.h"

void Func_0803fb60(void);

void Func_0803fd14(void)
{
    Scheduler_RemoveCallback(Func_0803fb60);
    Runtime_ReleaseHeapBlock(208);
}
