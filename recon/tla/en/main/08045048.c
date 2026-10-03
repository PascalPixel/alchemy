#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "SCENE.H"
#include "RESOURCE.H"
#include "RENDER_INPUT.H"
void Runtime_RemapBytesByTableFar(void *, s32);
u32 Resource_DecodeByteLz(const void *, void *);

/* graphics/resource/RenderResource_LoadFrame.c */


void RenderResource_LoadFrame(s32 index, s32 value, s32 flag)
{
    s32 size = 1024;
    void *buffer = Runtime_AllocateBlock(14, size);
    u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

    if (value <= 95) {
        Resource_DecodeByteLz((void *)((u32)base + base[index]), buffer);
        if (flag != 0)
            Runtime_RemapBytesByTableFar(buffer, 768);
        VramBlock_LoadCached(value, size, buffer);
        Runtime_ReleaseHeapBlock(14);
    }
}
