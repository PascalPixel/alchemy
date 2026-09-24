#include "TYPES.H"
#include "SYSTEM.H"

extern u8 Data_08011bf5;
void Scheduler_RemoveCallback(void *);

void Runtime_ScheduleCallbackAndReleaseBlock28(void)
{
    Scheduler_RemoveCallback(&Data_08011bf5);
    Runtime_ReleaseHeapBlock(0x1C);
}
