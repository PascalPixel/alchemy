#include "TYPES.H"
#include "RESOURCE.H"
#include "RENDER_INPUT.H"
#include "RUNTIME_MEM.H"

extern char ResourceId_CommandIcons;

void Runtime_RemapBytesByTableFar(void *buffer, s32 size);
void Resource_DecodeByteLzInRam(void *source, void *destination);

/* Loads command icon index into VRAM block value, remapping its colours
   when flag is set. */
void RenderResource_LoadFrame(s32 index, s32 value, s32 flag)
{
    s32 size = 1024;
    void *buffer = Runtime_AllocateBlock(56, size);
    u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

    if (value <= 95) {
        Resource_DecodeByteLzInRam((void *)((u32)base + base[index]), buffer);
        if (flag != 0)
            Runtime_RemapBytesByTableFar(buffer, 768);
        VramBlock_LoadCached(value, size, buffer);
        Runtime_ReleaseHeapBlock(56);
    }
}

struct RenderOutput *RenderResource_CreateFrame(
    s32 arg0,
    s32 arg1,
    struct RenderInput *arg2,
    s32 arg3,
    s32 arg4)
{
    s32 index;
    struct RenderOutput *entity;

    index = Resource_FindFreeEntry();
    entity = NULL;
    if (index != 0x60) {
        RenderResource_LoadFrame(arg0, index, arg1);
        entity = RenderOutput_Create(index, 0x80000000, arg2, arg3, arg4);
        ((u8 *)&entity->packed)[1] |= 0x20;
        entity->sentinel = 0xfb;
    }
    return entity;
}
