#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "RUNTIME_MEM.H"

void UiWork_Finalize(void *, s32);

/* ☀️'s: finishes the window that heap slot 16 holds and frees the slot. */
void UiWork_FinalizeAndReleaseBlock16(void)
{
    UiWork_Finalize(*(void **)Ram_HeapSlots->window_layout, 1);
    Runtime_ReleaseHeapBlock(64);
}
