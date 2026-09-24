#include "INN_RUNTIME.H"

s32 Runtime_ReleaseHeapBlock(s32);
s32 Resource_ResetEntry(u16);
s32 Scheduler_RemoveCallback(s32);
s32 UiWork_FinalizePendingCoreFar();
void Shop_StepCursor(void);

void Inn_Cleanup(void)
{
    struct InnRuntimeState *state;

    state = gMenuWork;
    Scheduler_RemoveCallback((s32)Shop_StepCursor);
    UiWork_FinalizePendingCoreFar();
    Resource_ResetEntry(state->resource_entries[0]);
    Resource_ResetEntry(state->resource_entries[1]);
    Resource_ResetEntry(state->resource_entries[2]);
    Resource_ResetEntry(state->resource_entries[3]);
    Resource_ResetEntry(state->resource_entries[4]);
    Resource_ResetEntry(state->resource_entries[5]);
    Runtime_ReleaseHeapBlock(0x37);
}
