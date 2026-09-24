#include "TYPES.H"
#include "GLOBAL_CELLS.H"

s32 Runtime_ReleaseHeapBlock(s32);
void UiWork_Finalize(struct Work *work, s32 release);

void UiWork_FinalizeAndReleaseBlock16(void)
{
    UiWork_Finalize(**(s32 **)ADDR_03001E90, 1);
    Runtime_ReleaseHeapBlock(0x10);
}
