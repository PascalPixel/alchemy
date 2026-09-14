#include "TYPES.H"
#include "SCENE.H"

s32 ScheduleCallbackAfterFrames(s32, s32);
extern u8 gRom;

void Scheduler_ScheduleCallbackCAfterFrames(void)
{
    ScheduleCallbackAfterFrames((s32)&gRom, 0xC80);
}
