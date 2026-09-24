#include "TYPES.H"
#include "SCENE.H"

s32 ScheduleCallback(s32);
extern u8 gRom;

void Runtime_ScheduleCallbackAndReleaseBlock32B(void)
{
    ScheduleCallback((s32)&gRom);
    Runtime_ReleaseHeapBlock(0x20);
}
