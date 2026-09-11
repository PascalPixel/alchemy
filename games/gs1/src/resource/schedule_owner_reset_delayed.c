#include "types.h"
#include "scene.h"
#include "abi/resource/schedule_owner_reset_delayed.h"

s32 ScheduleCallbackAfterFrames(s32, s32);

void Resource_ScheduleOwnerResetDelayed(void)
{
    ScheduleCallbackAfterFrames((s32)Sys_Run, 0xC80);
}
