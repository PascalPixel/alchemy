#include "TYPES.H"
#include "SCENE.H"

s32 Scheduler_AddOrUpdateCallback(s32, s32);
extern u8 gRom;

void Scheduler_ScheduleCallbackCAfterFrames(void)
{
    Scheduler_AddOrUpdateCallback((s32)&gRom, 0xC80);
}
