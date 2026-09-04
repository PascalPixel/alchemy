#include "types.h"

#define Runtime_ScheduleCallbackAndReleaseBlock20A Func_0801d9bc

s32 Func_08002dd8(s32);
s32 ScheduleCallback(s32);
extern u8 Data_0801d94d;

void Runtime_ScheduleCallbackAndReleaseBlock20A(void) {
    ScheduleCallback((s32)&Data_0801d94d);
    Func_08002dd8(0x14);
}
