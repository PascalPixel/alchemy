#include "types.h"

#define Runtime_ScheduleCallbackAndReleaseBlock32B Func_080911e8

s32 Func_08002dd8(s32);
s32 ScheduleCallback(s32);
extern u8 Data_080908e1;

void Runtime_ScheduleCallbackAndReleaseBlock32B(void) {
    ScheduleCallback((s32)&Data_080908e1);
    Func_08002dd8(0x20);
}
