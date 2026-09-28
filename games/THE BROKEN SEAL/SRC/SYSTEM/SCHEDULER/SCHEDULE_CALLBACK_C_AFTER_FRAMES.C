#include "TYPES.H"
#include "SCENE.H"

s32 Scheduler_AddOrUpdateCallback(s32, s32);
extern u8 Menu_RunSelection;

void Scheduler_ScheduleCallbackCAfterFrames(void)
{
    Scheduler_AddOrUpdateCallback((s32)&Menu_RunSelection, 0xC80);
}
