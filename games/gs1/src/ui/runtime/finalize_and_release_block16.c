#include "types.h"
#include "global_cells.h"

#define UiWork_FinalizeAndReleaseBlock16 Func_0801f5d4

s32 Func_08002dd8(s32);
void UiWork_Finalize(struct Work *work, s32 release);

void UiWork_FinalizeAndReleaseBlock16(void) {
    UiWork_Finalize(**(s32 **)ADDR_03001E90, 1);
    Func_08002dd8(0x10);
}
