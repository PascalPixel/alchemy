#include "types.h"
#include "scene.h"
#include "abi/ui/icon/copy_resource_to_slot.h"

s32 Resource_CopyData(s32, s32, s32);
s32 Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);

s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2)
{
    s32 work;

    work = Runtime_AllocateHeapBlock(0x11, 0x608);
    Ui_Apply(arg0, arg1);
    Resource_CopyData(arg2, 0x80, work + 0x400);
    Ui_Check(0x11);
    return 1;
}
