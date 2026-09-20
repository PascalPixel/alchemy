#include "TYPES.H"

extern u8 Data_08011bf5;
void Func_08002dd8(s32);
#define Runtime_ReleaseHeapBlock Func_08002dd8
void ScheduleCallback(void *);

void Runtime_ScheduleCallbackAndReleaseBlock28(void)
{
    ScheduleCallback(&Data_08011bf5);
    Runtime_ReleaseHeapBlock(0x1C);
}
