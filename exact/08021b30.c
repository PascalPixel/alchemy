#include "types.h"

s32 Func_08002dd8(s32);
s32 Func_080040d0(s32 index, s32 value);
void *Func_080048f4(s32 arg0, s32 arg1);
void Func_0801a3d0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

s32 Func_08021b30(s32 resource, s32 index)
{
    s32 sp4;
    s32 sp8;
    s32 temp_r5;
    u8 *temp_r5_2;

    temp_r5_2 = Func_080048f4(0x11, 0x608);
    sp8 = index;
    Func_0801a3d0(resource, 0, &sp8, &sp4, 1);
    temp_r5 = Func_080040d0(index, (s32)(temp_r5_2 + 0x400));
    Func_08002dd8(0x11);
    return temp_r5;
}
