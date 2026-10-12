#include "types.h"
#include "graphics.h"
#include "system.h"
#include "resource.h"
#include "mem.h"
#include "glyph.h"
#include "dma.h"
#include "io_reg.h"

#define Resource_DecodeByteLzInRam ((void (*)(void *, void *))Resource_DecodeByteLzInRam)

extern char ResourceId_CommandIcons;

s32 Resource_LoadIndexedIntoBuffer(s32 arg0, s32 arg1)
{
    GlyphTransfer *buffer = Runtime_AllocateHeapBlock(68, sizeof(GlyphTransfer));
    u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
    u8 **slot = &buffer->encoded;
    void *target = (void *)((u32)base + base[arg1]);
    s32 ret;

    *slot = target;
    Resource_DecodeByteLzInRam(target, buffer);
    ret = Resource_GetBuffer(arg0, (s32)buffer);
    Runtime_ReleaseHeapBlock(68);
    return ret;
}
