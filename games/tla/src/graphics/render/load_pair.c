#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "mem.h"
#include "types.h"
#include "render_input.h"

#define Resource_DecodeByteLzInRam ((void (*)(void*,void*))Resource_DecodeByteLzInRam)



void RenderResource_LoadPair(s32 group_index, s32 resource_index)
{
    void *staging_buffer = Runtime_AllocateBlock(56, 0x400);
    s32 resource_address = RenderResource_PairSourceTable[group_index];

    if (resource_index <= 0x5F) {
        Resource_DecodeByteLzInRam((void *)resource_address, staging_buffer);
        VramBlock_LoadCached(resource_index, 0x200, staging_buffer);
        Runtime_ReleaseHeapBlock(56);
    }
}

struct RenderOutput *RenderResource_CreatePair(
    s32 group,
    struct RenderInput *input,
    s32 x,
    s32 y)
{
    s32 index;
    struct RenderOutput *first;
    struct RenderOutput *second;

    index = Resource_FindFreeEntry();
    if (index > 95)
        return NULL;

    RenderResource_LoadPair(group, index);
    first = RenderOutput_Create(index, 0x80004000, input, x, y);
    first->sentinel = 0xFD;
    second = RenderOutput_Create(index, 0x80004000, input, x + 32, y);
    second->sentinel = 0xFD;
    second->oam.index += 8;
    return first;
}
