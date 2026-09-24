#include "TYPES.H"

s32 Runtime_ReleaseHeapBlock(s32);
s32 ScheduleCallback(s32);
extern u8 Data_0801cf49;

void Runtime_ScheduleCallbackAndReleaseBlock20B(void)
{
    ScheduleCallback((s32)&Data_0801cf49);
    Runtime_ReleaseHeapBlock(0x14);
}
