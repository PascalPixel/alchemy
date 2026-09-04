#include "types.h"

#define Scheduler_ScheduleCallbackBAfterFrames Func_08011be0

s32 ScheduleCallbackAfterFrames(s32, s32);
extern u8 Data_08011bf5;

void Scheduler_ScheduleCallbackBAfterFrames(void)
{
    ScheduleCallbackAfterFrames((s32)&Data_08011bf5, 0xc80);
}
