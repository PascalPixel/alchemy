#include "TYPES.H"

s32 Runtime_ReleaseHeapBlock(s32);
s32 Scheduler_RemoveCallback(s32);
void GraphicsPalette_LoadSelectionResourcesAndAdvance(void);

void Runtime_ScheduleCallbackAndReleaseBlock20B(void)
{
    Scheduler_RemoveCallback((s32)GraphicsPalette_LoadSelectionResourcesAndAdvance);
    Runtime_ReleaseHeapBlock(0x14);
}
