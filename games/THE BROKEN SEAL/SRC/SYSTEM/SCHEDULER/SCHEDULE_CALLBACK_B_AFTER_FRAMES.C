#include "TYPES.H"
#include "PALQUEUE.H"
#include "CALLBACK_SCHEDULER.H"


void Scheduler_ScheduleCallbackBAfterFrames(void)
{
    Scheduler_AddOrUpdateCallback((s32)Func_08011bf4, 0xc80);
}
