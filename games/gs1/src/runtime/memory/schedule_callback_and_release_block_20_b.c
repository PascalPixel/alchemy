#include "types.h"
#include "scene.h"

s32 ScheduleCallback(s32);
extern u8 gRom;

void Runtime_ScheduleCallbackAndReleaseBlock20B(void)
{
    ScheduleCallback((s32)&gRom);
    Sys_Check(0x14);
}
