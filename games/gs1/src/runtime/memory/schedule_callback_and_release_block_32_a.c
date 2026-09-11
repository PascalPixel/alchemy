#include "types.h"
#include "scene.h"
#include "abi/runtime/memory/schedule_callback_and_release_block_32_a.h"

s32 ScheduleCallback(s32);

extern u8 gRom;

void Runtime_ScheduleCallbackAndReleaseBlock32A(void)
{
    ScheduleCallback((s32)&gRom);
    Sys_Check(0x20);
}
