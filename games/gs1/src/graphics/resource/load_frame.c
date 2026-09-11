#include "types.h"
#include "scene.h"
#include "abi/graphics/resource/load_frame.h"
#include "resource.h"

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

void Resource_CopyData(s32, s32, void *);

extern unsigned char gVal[];

void RenderResource_LoadFrame(s32 index, s32 value, s32 flag)
{
    s32 size = 1024;
    void *buffer = Runtime_AllocateBlock(14, size);
    u16 *base = GetResource((s32)gVal);

    if (value <= 95) {
        Sys_Apply((void *)((u32)base + base[index]), buffer);
        if (flag != 0)
            Sys_Apply2(buffer, 768);
        Resource_CopyData(value, size, buffer);
        Sys_Do(14);
    }
}
