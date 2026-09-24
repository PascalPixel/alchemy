#include "TYPES.H"

s32 Runtime_ReleaseHeapBlock(s32);
s32 ScheduleCallback(s32);
void GraphicsPalette_LoadSelectionResourcesAndAdvance(void);

void Runtime_ScheduleCallbackAndReleaseBlock20B(void)
{
    ScheduleCallback((s32)GraphicsPalette_LoadSelectionResourcesAndAdvance);
    Runtime_ReleaseHeapBlock(0x14);
}
