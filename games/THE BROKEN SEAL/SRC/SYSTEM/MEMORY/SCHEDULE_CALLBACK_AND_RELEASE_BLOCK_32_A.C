#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"

/* runtime/memory/schedule_callback_and_release_block_32_a.c */
s32 Scheduler_RemoveCallback(s32);

extern u8 gRom;

void Runtime_ScheduleCallbackAndReleaseBlock32A(void)
{
    Scheduler_RemoveCallback((s32)&gRom);
    Runtime_ReleaseHeapBlock(0x20);
}
