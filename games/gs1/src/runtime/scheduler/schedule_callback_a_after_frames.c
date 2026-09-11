#include "types.h"
#include "scene.h"

s32 ScheduleCallbackAfterFrames(s32, s32);

void Scheduler_ScheduleCallbackAAfterFrames(void)
{
    ScheduleCallbackAfterFrames(0x0801FD35, 0xC80);
}
