#include "TYPES.H"

s32 Scheduler_AddOrUpdateCallback(s32, s32);
extern u8 Func_08011bf4;

void Scheduler_ScheduleCallbackBAfterFrames(void)
{
    Scheduler_AddOrUpdateCallback((s32)&Func_08011bf4, 0xc80);
}
