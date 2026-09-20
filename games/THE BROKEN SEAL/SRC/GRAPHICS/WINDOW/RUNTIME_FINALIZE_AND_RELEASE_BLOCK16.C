#include "TYPES.H"
#include "GLOBAL_CELLS.H"

s32 Func_08002dd8(s32);
#define Runtime_ReleaseHeapBlock Func_08002dd8
void UiWork_Finalize(struct Work *work, s32 release);

void UiWork_FinalizeAndReleaseBlock16(void)
{
    UiWork_Finalize(**(s32 **)ADDR_03001E90, 1);
    Runtime_ReleaseHeapBlock(0x10);
}
