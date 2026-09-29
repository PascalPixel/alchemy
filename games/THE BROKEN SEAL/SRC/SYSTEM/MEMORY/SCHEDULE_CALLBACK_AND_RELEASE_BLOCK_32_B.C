#include "TYPES.H"
#include "SCENE.H"
s32 Runtime_ReleaseHeapBlock(s32);

s32 Scheduler_RemoveCallback(s32);
extern u8 BattlePalette_UpdateBlend;

void Runtime_ScheduleCallbackAndReleaseBlock32B(void)
{
    Scheduler_RemoveCallback((s32)&BattlePalette_UpdateBlend);
    Runtime_ReleaseHeapBlock(0x20);
}
