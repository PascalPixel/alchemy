#include "TYPES.H"
#include "SCENE.H"
s32 Runtime_ReleaseHeapBlock(s32);

s32 Scheduler_RemoveCallback(s32);
extern u8 Func_080908e0;

void Runtime_ScheduleCallbackAndReleaseBlock32B(void)
{
    Scheduler_RemoveCallback((s32)&Func_080908e0);
    Runtime_ReleaseHeapBlock(0x20);
}
