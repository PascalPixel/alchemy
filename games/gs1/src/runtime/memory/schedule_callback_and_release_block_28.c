#include "types.h"

#define Runtime_ScheduleCallbackAndReleaseBlock28 Func_08011bc8

extern u8 Data_08011bf5;
void Func_08002dd8(s32);
void ScheduleCallback(void *);

void Runtime_ScheduleCallbackAndReleaseBlock28(void)
{
    ScheduleCallback(&Data_08011bf5);
    Func_08002dd8(0x1C);
}
