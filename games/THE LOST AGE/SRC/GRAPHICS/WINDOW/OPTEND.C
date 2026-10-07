#include "TYPES.H"
#include "MEM.H"
#include "CALLBACK_SCHEDULER.H"

void Func_0803fb60(void);

void Func_0803fd14(void)
{
    Scheduler_RemoveCallback(Func_0803fb60);
    Runtime_ReleaseHeapBlock(208);
}
