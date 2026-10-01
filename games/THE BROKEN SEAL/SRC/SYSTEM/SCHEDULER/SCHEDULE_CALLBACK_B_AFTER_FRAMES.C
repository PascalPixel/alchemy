#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"

extern u8 Func_08011bf4;

void Scheduler_ScheduleCallbackBAfterFrames(void)
{
    Scheduler_AddOrUpdateCallback((s32)&Func_08011bf4, 0xc80);
}
