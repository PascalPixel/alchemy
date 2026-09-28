#include "TYPES.H"
#include "SCENE.H"
s32 Runtime_ReleaseHeapBlock(s32);

s32 Scheduler_RemoveCallback(s32);
extern u8 Menu_RunSelectedWorkspaceEntry;

void Runtime_ScheduleCallbackAndReleaseBlock20A(void)
{
    Scheduler_RemoveCallback((s32)&Menu_RunSelectedWorkspaceEntry);
    Runtime_ReleaseHeapBlock(0x14);
}
