#include "types.h"

s32 ScheduleCallback(s32);
s32 Func_08002dd8(s32);
extern u8 Data_080f2f11;

void Runtime_ScheduleCallbackAndReleaseBlock32A(void) {
    ScheduleCallback((s32)&Data_080f2f11);
    Func_08002dd8(0x20);
}
