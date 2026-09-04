#include "types.h"

#define Scheduler_ScheduleCallbackCAfterFrames Func_08028edc

s32 ScheduleCallbackAfterFrames(s32, s32);
extern u8 Data_08028f99;

void Scheduler_ScheduleCallbackCAfterFrames(void) {
    ScheduleCallbackAfterFrames((s32)&Data_08028f99, 0xC80);
}
