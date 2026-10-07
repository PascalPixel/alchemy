#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "RENDER_INPUT.H"

#define Resource_DecodeByteLzInRam ((void (*)(void*,void*))Resource_DecodeByteLzInRam)



void RenderResource_LoadPair(s32 group_index, s32 resource_index)
{
    void *staging_buffer = Runtime_AllocateBlock(56, 1024);
    s32 resource_address = RenderResource_PairSourceTable[group_index];

    if (resource_index <= 95) {
        Resource_DecodeByteLzInRam((void *)resource_address, staging_buffer);
        VramBlock_LoadCached(resource_index, 512, staging_buffer);
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
    first->sentinel = 253;
    second = RenderOutput_Create(index, 0x80004000, input, x + 32, y);
    second->sentinel = 253;
    second->table.bits.index += 8;
    return first;
}
