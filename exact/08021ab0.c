#include "types.h"

s32 Func_08002dd8(s32);
s32 Func_080040d0(s32 index, s32 value);
void *Func_080048f4(s32 arg0, s32 arg1);
void Func_0801a4c0(u32 index);

s32 Func_08021ab0(s32 resource, s32 index)
{
    s32 result;
    u8 *work;

    work = Func_080048f4(0x11, 0x608);
    Func_0801a4c0(resource);
    result = Func_080040d0(index, (s32)(work + 0x400));
    Func_08002dd8(0x11);
    return result;
}
