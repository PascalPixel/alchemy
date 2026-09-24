#include "TYPES.H"

s32 Scheduler_AddOrUpdateCallback(s32, s32);
extern u8 Data_08011bf5;

void Scheduler_ScheduleCallbackBAfterFrames(void)
{
    Scheduler_AddOrUpdateCallback((s32)&Data_08011bf5, 0xc80);
}
