#include "types.h"

s32 ScheduleCallbackAfterFrames(s32, s32);
void Func_0801a98c(void);

void Resource_ScheduleOwnerResetDelayed(void) {
    ScheduleCallbackAfterFrames((s32)Func_0801a98c, 0xC80);
}
