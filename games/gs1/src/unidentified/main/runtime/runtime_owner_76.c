#include "types.h"

s32 Func_08002dd8(s32);
s32 Resource_GetBuffer(s32 index, s32 value);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
s32 Func_0801a088(s32, s32);

s32 Func_08021af0(s32 resource, s32 index)
{
    s32 result;
    u8 *work;

    work = Runtime_AllocateBlock(0x11, 0x608);
    Func_0801a088(resource, 0x1a);
    result = Resource_GetBuffer(index, (s32)(work + 0x400));
    Func_08002dd8(0x11);
    return result;
}
