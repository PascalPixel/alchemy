#include "types.h"

s32 ScheduleCallbackAfterFrames(s32, s32);
extern u8 Data_08028f99;

void Func_08028edc(void) {
    ScheduleCallbackAfterFrames((s32)&Data_08028f99, 0xC80);
}
