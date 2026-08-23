#include "types.h"

s32 ScheduleCallbackAfterFrames(s32, s32);
s32 Func_080770d0(s32);
void Func_080a2444(void);

void Func_080a2474(void) {
    Func_080770d0(0x150);
    ScheduleCallbackAfterFrames((s32)Func_080a2444, 0xC80);
}
