#include "inn_runtime.h"

s32 Func_08002dd8(s32);
s32 Resource_ResetEntry(u16);
s32 ScheduleCallback(s32);
s32 UiWork_FinalizePending();
extern u8 Data_080b00f5;

void Inn_CleanupResources(void) {
    struct InnRuntimeState *state;

    state = Data_03001f2c;
    ScheduleCallback((s32)&Data_080b00f5);
    UiWork_FinalizePending();
    Resource_ResetEntry(state->resource_entries[0]);
    Resource_ResetEntry(state->resource_entries[1]);
    Resource_ResetEntry(state->resource_entries[2]);
    Resource_ResetEntry(state->resource_entries[3]);
    Resource_ResetEntry(state->resource_entries[4]);
    Resource_ResetEntry(state->resource_entries[5]);
    Func_08002dd8(0x37);
}
