#include "types.h"
#include "scene.h"
#include "abi/runtime/scheduler/schedule_callback_b_after_frames.h"

s32 ScheduleCallbackAfterFrames(s32, s32);
extern u8 gRom;

void Scheduler_ScheduleCallbackBAfterFrames(void)
{
    ScheduleCallbackAfterFrames((s32)&gRom, 0xc80);
}
