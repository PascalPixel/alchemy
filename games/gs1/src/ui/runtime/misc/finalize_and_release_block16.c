#include "types.h"
#include "scene.h"
#include "abi/ui/runtime/misc/finalize_and_release_block16.h"
#include "global_cells.h"

void UiWork_Finalize(struct Work *work, s32 release);

void UiWork_FinalizeAndReleaseBlock16(void)
{
    UiWork_Finalize(**(s32 **)ADDR_03001E90, 1);
    Ui_Check(0x10);
}
