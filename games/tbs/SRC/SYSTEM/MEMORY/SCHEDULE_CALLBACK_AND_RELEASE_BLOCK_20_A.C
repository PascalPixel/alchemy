#include "TYPES.H"
#include "SCENE.H"

s32 ScheduleCallback(s32);
extern u8 gRom;

void Runtime_ScheduleCallbackAndReleaseBlock20A(void)
{
    ScheduleCallback((s32)&gRom);
    Sys_Check(0x14);
}
