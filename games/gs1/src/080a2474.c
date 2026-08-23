#include "types.h"

s32 ScheduleCallbackAfterFrames(s32, s32);
s32 GameFlag_Clear(s32);
void Func_080a2444(void);

void Func_080a2474(void) {
    GameFlag_Clear(0x150);
    ScheduleCallbackAfterFrames((s32)Func_080a2444, 0xC80);
}
