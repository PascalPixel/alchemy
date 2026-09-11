#include "scene.h"
#include "inn.h"

s32 Resource_ResetEntry(u16);
s32 ScheduleCallback(s32);
s32 UiWork_FinalizePending();
extern u8 gRom;

void Inn_Cleanup(void)
{
    struct InnState *state;

    state = gIw;
    ScheduleCallback((s32)&gRom);
    UiWork_FinalizePending();
    Resource_ResetEntry(state->resource_entries[0]);
    Resource_ResetEntry(state->resource_entries[1]);
    Resource_ResetEntry(state->resource_entries[2]);
    Resource_ResetEntry(state->resource_entries[3]);
    Resource_ResetEntry(state->resource_entries[4]);
    Resource_ResetEntry(state->resource_entries[5]);
    Sys_Check(0x37);
}
