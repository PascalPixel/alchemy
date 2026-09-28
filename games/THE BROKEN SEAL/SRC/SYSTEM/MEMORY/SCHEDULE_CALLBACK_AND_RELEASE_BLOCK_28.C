#include "TYPES.H"
#include "SYSTEM.H"

extern u8 Func_08011bf4;
void Scheduler_RemoveCallback(void *);

void Runtime_ScheduleCallbackAndReleaseBlock28(void)
{
    Scheduler_RemoveCallback(&Func_08011bf4);
    Runtime_ReleaseHeapBlock(0x1C);
}
