#include "types.h"
#include "scene.h"

/* graphics/vram/upload_block.c */
struct State {
    u8 unknown[156];
    u32 context;
    u32 source;
};

extern struct State gIw;

u32 Graphics_UploadVramBlock(void)
{
    void *source = (void *)gIw.source;
    u8 *context = (u8 *)gIw.context;

    if (source != 0) {
        u32 *active = (u32 *)(context + 0x13C0);

        if (*active != 0) {
            *active = 0;
            return Sys_Place(source, (void *)0x06004000, 0x4000);
        }
    }
    return (u32)source;
}

/* graphics/vram/reset_block_and_release_heap_blocks.c */
/*
 * Sys_SetMode names a bx rN veneer slot, so this is an indirect call
 * through the register loaded just before it -- the relocated routine at
 * 0x03000164. Its argument count is not established.
 */

s32 ScheduleCallback(u32);
extern u8 gRom;
extern u8 gRom2;

s32 Graphics_ResetVramBlockAndReleaseHeapBlocks(s32 unused0, s32 unused1, s32 mode)
{
    Sys_SetMode(0x06004000, 0x4000, mode, 0x03000164);
    Sys_Do(47);
    Sys_Do(46);
    Sys_Do(40);
    Sys_Do(39);
    *(u16 *)0x04000000 = 0x1341;
    ScheduleCallback((u32)&gRom);
    return ScheduleCallback((u32)&gRom2);
}

/* graphics/color/scale_rgb555_clamped.c */
s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale, s32 count)
{
    s32 remaining;
    u32 maskRed;
    u32 maskGreen;
    u32 maskBlue;
    u32 pixel;
    u32 red;
    u32 green;
    u32 blue;

    if (scale > 0x10000) {
        scale = 0x10000;
    }
    if (count > 0) {
        maskRed = 0x1f;
        maskGreen = 0x3e0;
        maskBlue = 0x7c00;
        remaining = count;
        do {
            pixel = *source;
            red = pixel & maskRed;
            green = pixel & maskGreen;
            blue = maskBlue & pixel;
            red *= scale;
            green *= scale;
            blue *= scale;
            pixel = ((red >> 16) & maskRed) | ((green >> 16) & maskGreen);
            pixel |= (blue >> 16) & maskBlue;
            *destination = pixel;
            source++;
            destination++;
            remaining--;
        } while (remaining != 0);
    }
    return 0;
}
