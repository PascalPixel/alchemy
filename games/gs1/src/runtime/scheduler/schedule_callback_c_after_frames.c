#include "types.h"
#include "scene.h"

s32 ScheduleCallbackAfterFrames(s32, s32);
extern u8 gRom;

void Scheduler_ScheduleCallbackCAfterFrames(void)
{
    ScheduleCallbackAfterFrames((s32)&gRom, 0xC80);
}
