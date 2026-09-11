#include "types.h"
#include "scene.h"
#include "abi/ui/render/build_pattern_to_slot.h"


s32 Resource_CopyData(s32, s32, s32);
s32 Runtime_AllocateHeapBlock(s32 no, s32 arg1);

s32 Ui_BuildPatternToSlot(s32 no, s32 arg1, s32 slot)
{
    s32 work;

    work = Runtime_AllocateHeapBlock(0x11, 0x608);
    Ui_Do(no);
    Resource_CopyData(slot, 0x80, work + 0x400);
    Ui_Check(0x11);
    return 1;
}
