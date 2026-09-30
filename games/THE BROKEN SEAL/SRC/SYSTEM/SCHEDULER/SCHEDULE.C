#include "TYPES.H"

void PaletteGlow_UpdateSine(void);

s32 Scheduler_AddOrUpdateCallback(s32, s32);

void Scheduler_ScheduleCallbackAAfterFrames(void)
{
    Scheduler_AddOrUpdateCallback((s32)PaletteGlow_UpdateSine, 0xC80);
}

void PaletteGlow_UpdateSine(void);

s32 Scheduler_RemoveCallback(s32);

void Scheduler_ScheduleCallbackA(void)
{
    Scheduler_RemoveCallback((s32)PaletteGlow_UpdateSine);
}
