#include "types.h"

s32 Func_08002dd8(s32);
s32 ScheduleCallback(s32);
extern u8 Data_0801cf49;

void Func_0801d0f0(void) {
    ScheduleCallback((s32)&Data_0801cf49);
    Func_08002dd8(0x14);
}
