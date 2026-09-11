#include "types.h"
#include "scene.h"
#include "resource.h"

extern u8 gVal[];

s32 Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);

s32 Resource_GetBuffer(s32 index, s32 value);

s32 Resource_LoadIndexedIntoBuffer(s32 arg0, s32 arg1)
{
    void *buffer = Runtime_AllocateHeapBlock(0x11, 0x608);
    u16 *base = GetResource((s32)gVal);
    void **slot = (void **)((u32)buffer + 0x604);
    void *target = (void *)((u32)base + base[arg1]);
    s32 ret;

    *slot = target;
    Sys_Apply(target, buffer);
    ret = Resource_GetBuffer(arg0, (s32)buffer);
    Sys_Do(0x11);
    return ret;
}
