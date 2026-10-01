#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"

extern u8 IwramClearWords[];

/*
 * _call_via_r3 names a bx rN veneer slot, so this is an indirect call
 * through the register loaded just before it -- the relocated routine at
 * 0x03000164. Its argument count is not established.
 */
void _call_via_r3(u32, s32, s32, u32);
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

s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale, s32 count)
{
    s32 remaining;
    u32 mask_red;
    u32 mask_green;
    u32 mask_blue;
    u32 pixel;
    u32 red;
    u32 green;
    u32 blue;

    if (scale > 0x10000)
        scale = 0x10000;
    if (count > 0) {
        mask_red = 0x1f;
        mask_green = 0x3e0;
        mask_blue = 0x7c00;
        remaining = count;
        do {
            pixel = *source;
            red = pixel & mask_red;
            green = pixel & mask_green;
            blue = mask_blue & pixel;
            red *= scale;
            green *= scale;
            blue *= scale;
            pixel = ((red >> 16) & mask_red) | ((green >> 16) & mask_green);
            pixel |= (blue >> 16) & mask_blue;
            *destination = pixel;
            source++;
            destination++;
            remaining--;
        } while (remaining != 0);
    }
    return 0;
}
