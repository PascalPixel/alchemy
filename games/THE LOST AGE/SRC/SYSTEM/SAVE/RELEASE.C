#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "RUNTIME_MEM.H"
#include "UIFLOW.H"

void UiWork_FinalizeAndReleaseBlock16(void)
{
    UiWork_Finalize(*(void **)Ram_HeapSlots->window_layout, 1);
    Runtime_ReleaseHeapBlock(64);
}
