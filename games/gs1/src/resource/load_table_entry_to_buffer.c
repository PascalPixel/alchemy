#include "types.h"

#define Resource_LoadTableEntryToBuffer Func_08021ab0

s32 Func_08002dd8(s32);
s32 Resource_GetBuffer(s32 index, s32 value);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void Func_0801a4c0(u32 index);

s32 Resource_LoadTableEntryToBuffer(s32 resource, s32 index)
{
    s32 result;
    u8 *work;

    work = Runtime_AllocateBlock(0x11, 0x608);
    Func_0801a4c0(resource);
    result = Resource_GetBuffer(index, (s32)(work + 0x400));
    Func_08002dd8(0x11);
    return result;
}
