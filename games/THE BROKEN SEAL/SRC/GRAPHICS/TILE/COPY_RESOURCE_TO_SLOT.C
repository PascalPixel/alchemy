#include "TYPES.H"

s32 Runtime_ReleaseHeapBlock(s32);
s32 VramBlock_LoadCached(s32, s32, s32);
s32 Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);
s32 ItemIcon_Compose(s32, s32);

s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2)
{
    s32 work;

    work = Runtime_AllocateHeapBlock(0x11, 0x608);
    ItemIcon_Compose(arg0, arg1);
    VramBlock_LoadCached(arg2, 0x80, work + 0x400);
    Runtime_ReleaseHeapBlock(0x11);
    return 1;
}
