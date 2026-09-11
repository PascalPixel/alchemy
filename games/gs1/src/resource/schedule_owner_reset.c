#include "types.h"
#include "scene.h"
#include "abi/resource/schedule_owner_reset.h"

s32 ScheduleCallback(s32);

void Resource_ScheduleOwnerReset(void)
{
    ScheduleCallback((s32)Sys_Run);
}
