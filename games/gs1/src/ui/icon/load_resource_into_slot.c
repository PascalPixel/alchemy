#include "types.h"

s32 Func_08002dd8(s32);
s32 Resource_GetBuffer(s32 index, s32 value);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
s32 Func_080153d0(s32, s32, s32 *, s32 *, s32);

s32 UiIcon_LoadResourceIntoSlot(s32 arg0, s32 arg1) {
    s32 sp4;
    s32 sp8;
    s32 temp_r5;
    s32 temp_r5_2;

    temp_r5_2 = Runtime_AllocateBlock(0x11, 0x608);
    sp8 = arg1;
    Func_080153d0(arg0, 0, &sp8, &sp4, 1);
    temp_r5 = Resource_GetBuffer(arg1, temp_r5_2 + 0x400);
    Func_08002dd8(0x11);
    return temp_r5;
}
