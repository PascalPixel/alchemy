#include "types.h"

#define UiIcon_CopyResourceToSlot Func_0801a2a4

s32 Func_08002dd8(s32);
s32 Resource_CopyData(s32, s32, s32);
s32 Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);
s32 Func_0801a088(s32, s32);

s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2) {
    s32 work;

    work = Runtime_AllocateHeapBlock(0x11, 0x608);
    Func_0801a088(arg0, arg1);
    Resource_CopyData(arg2, 0x80, work + 0x400);
    Func_08002dd8(0x11);
    return 1;
}
