#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
s32 Runtime_ReleaseHeapBlock(s32);

/* runtime/memory/schedule_callback_and_release_block_32_a.c */
s32 Scheduler_RemoveCallback(s32);

extern u8 TitlePalette_UpdateFade;

void Runtime_ScheduleCallbackAndReleaseBlock32A(void)
{
    Scheduler_RemoveCallback((s32)&TitlePalette_UpdateFade);
    Runtime_ReleaseHeapBlock(0x20);
}
