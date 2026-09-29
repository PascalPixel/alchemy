#include "TYPES.H"
#include "SYSTEM.H"
extern u8 IwramClearWords[];

/*
 * _call_via_r3 names a bx rN veneer slot, so this is an indirect call
 * through the register loaded just before it -- the relocated routine at
 * 0x03000164. Its argument count is not established.
 */
void _call_via_r3(u32, s32, s32, u32);
s32 Scheduler_RemoveCallback(u32);
extern u8 Graphics_UploadVramBlock;
extern u8 BattleFx_UpdateStarField;

s32 Graphics_ResetVramBlockAndReleaseHeapBlocks(s32 unused0, s32 unused1, s32 mode)
{
    _call_via_r3(0x06004000, 0x4000, mode, (u32)IwramClearWords);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    Runtime_ReleaseHeapBlock(40);
    Runtime_ReleaseHeapBlock(39);
    *(u16 *)0x04000000 = 0x1341;
    Scheduler_RemoveCallback((u32)&Graphics_UploadVramBlock);
    return Scheduler_RemoveCallback((u32)&BattleFx_UpdateStarField);
}
