#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"

void PaletteGlow_UpdateSine(void);

/* ☀️'s pair, which ⚓️ builds in every edition with its own order. */
void Scheduler_ScheduleCallbackAAfterFrames(void)
{
    Scheduler_AddOrUpdateCallback((s32)PaletteGlow_UpdateSine, 0x480);
}

void Scheduler_ScheduleCallbackA(void)
{
    Scheduler_RemoveCallback((u32)((s32)PaletteGlow_UpdateSine));
}
