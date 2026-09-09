#include "types.h"

/*
 * Func_080072f0 names a bx rN veneer slot, so this is an indirect call
 * through the register loaded just before it -- the relocated routine at
 * 0x03000164. Its argument count is not established.
 */
void Func_080072f0(u32, s32, s32, u32);
void Func_08002dd8(s32);
s32 ScheduleCallback(u32);
extern u8 Data_080c1439;
extern u8 Data_080c11ed;

s32 Graphics_ResetVramBlockAndReleaseHeapBlocks(s32 unused0, s32 unused1, s32 mode)
{
    Func_080072f0(0x06004000, 0x4000, mode, 0x03000164);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_08002dd8(40);
    Func_08002dd8(39);
    *(u16 *)0x04000000 = 0x1341;
    ScheduleCallback((u32)&Data_080c1439);
    return ScheduleCallback((u32)&Data_080c11ed);
}
