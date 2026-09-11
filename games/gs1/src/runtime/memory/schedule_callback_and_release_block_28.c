#include "types.h"
#include "scene.h"
#include "abi/runtime/memory/schedule_callback_and_release_block_28.h"

extern u8 gRom;

void ScheduleCallback(void *);

void Runtime_ScheduleCallbackAndReleaseBlock28(void)
{
    ScheduleCallback(&gRom);
    Sys_Do(0x1C);
}
