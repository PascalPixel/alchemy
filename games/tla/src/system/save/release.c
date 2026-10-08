#include "types.h"
#include "ram_buffer.h"
#include "mem.h"
#include "uiflow.h"

void UiWork_FinalizeAndReleaseBlock16(void)
{
    UiWork_Finalize(*(void **)Ram_HeapSlots->window_layout, 1);
    Runtime_ReleaseHeapBlock(64);
}
