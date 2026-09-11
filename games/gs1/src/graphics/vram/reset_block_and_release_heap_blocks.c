#include "types.h"
#include "scene.h"
#include "abi/graphics/vram/reset_block_and_release_heap_blocks.h"

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
