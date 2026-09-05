#include "types.h"

s32 Func_08002dd8(s32);
s32 Resource_GetBuffer(s32 index, s32 value);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void Func_0801a3d0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index)
{
    s32 out;
    s32 cur;
    s32 ret;
    u8 *work;

    work = Runtime_AllocateBlock(0x11, 0x608);
    cur = index;
    Func_0801a3d0(resource, 0, &cur, &out, 1);
    ret = Resource_GetBuffer(index, (s32)(work + 0x400));
    Func_08002dd8(0x11);
    return ret;
}
