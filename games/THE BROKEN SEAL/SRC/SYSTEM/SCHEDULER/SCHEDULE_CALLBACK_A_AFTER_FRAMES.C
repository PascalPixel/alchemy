#include "TYPES.H"

s32 Scheduler_AddOrUpdateCallback(s32, s32);

void Scheduler_ScheduleCallbackAAfterFrames(void)
{
    Scheduler_AddOrUpdateCallback(0x0801FD35, 0xC80);
}
