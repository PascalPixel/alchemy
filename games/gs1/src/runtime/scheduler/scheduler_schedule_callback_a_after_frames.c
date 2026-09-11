#include "types.h"
#include "scene.h"

/* runtime/scheduler/schedule_callback_a_after_frames.c */
s32 ScheduleCallbackAfterFrames(s32, s32);

void Scheduler_ScheduleCallbackAAfterFrames(void)
{
    ScheduleCallbackAfterFrames(0x0801FD35, 0xC80);
}

/* runtime/scheduler/schedule_callback_a.c */
s32 ScheduleCallback(s32);

void Scheduler_ScheduleCallbackA(void)
{
    ScheduleCallback(0x0801FD35);
}
