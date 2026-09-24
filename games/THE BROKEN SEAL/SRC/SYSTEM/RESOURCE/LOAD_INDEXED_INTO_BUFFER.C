#include "TYPES.H"
#include "RESOURCE.H"

extern u8 Data_000000f1[];

s32 Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);
u32 Resource_DecodeByteLz(const void *, void *);
s32 Resource_GetBuffer(s32 index, s32 value);
void Runtime_ReleaseHeapBlock(s32);

s32 Resource_LoadIndexedIntoBuffer(s32 arg0, s32 arg1)
{
    void *buffer = Runtime_AllocateHeapBlock(0x11, 0x608);
    u16 *base = GetResource((s32)Data_000000f1);
    void **slot = (void **)((u32)buffer + 0x604);
    void *target = (void *)((u32)base + base[arg1]);
    s32 ret;

    *slot = target;
    Resource_DecodeByteLz(target, buffer);
    ret = Resource_GetBuffer(arg0, (s32)buffer);
    Runtime_ReleaseHeapBlock(0x11);
    return ret;
}
