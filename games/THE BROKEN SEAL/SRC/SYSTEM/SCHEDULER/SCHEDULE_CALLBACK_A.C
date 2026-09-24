#include "TYPES.H"

s32 Scheduler_RemoveCallback(s32);

void Scheduler_ScheduleCallbackA(void)
{
    Scheduler_RemoveCallback(0x0801FD35);
}
