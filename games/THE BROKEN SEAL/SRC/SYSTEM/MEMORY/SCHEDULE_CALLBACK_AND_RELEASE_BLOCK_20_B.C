#include "TYPES.H"

s32 Func_08002dd8(s32);
#define Runtime_ReleaseHeapBlock Func_08002dd8
s32 ScheduleCallback(s32);
extern u8 Data_0801cf49;

void Runtime_ScheduleCallbackAndReleaseBlock20B(void)
{
    ScheduleCallback((s32)&Data_0801cf49);
    Runtime_ReleaseHeapBlock(0x14);
}
