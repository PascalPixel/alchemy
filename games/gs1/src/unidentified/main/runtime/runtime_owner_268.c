#include "types.h"

s32 ScheduleCallbackAfterFrames(s32, s32);
extern u8 Data_08011bf5;

void Func_08011be0(void)
{
    ScheduleCallbackAfterFrames((s32)&Data_08011bf5, 0xc80);
}
