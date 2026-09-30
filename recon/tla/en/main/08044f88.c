#include "TYPES.H"
#include "SCENE.H"
#include "RENDER_INPUT.H"
#include "RESOURCE.H"
u32 Resource_DecodeByteLz(const void *, void *);
void Runtime_ReleaseHeapBlock(s32);

/* graphics/resource/RenderOutput_LoadPair.c */
extern s32 RenderResource_PairSourceTable[];

void *RenderResource_CreatePair(
    s32 arg0,
    struct RenderInput *arg1,
    s32 arg2,
    s32 arg3)
{
    s32 index;
    struct RenderOutput *first;
    struct RenderOutput *second;

    index = Resource_FindFreeEntry();
    if (index > 95)
        return NULL;

    RenderResource_LoadPair(arg0, index);
    /* Two 32x16 OBJ sprites; the right half starts eight tiles later. */
    first = RenderOutput_Create(index, 0x80004000, arg1, arg2, arg3);
    first->sentinel = 0xFD;
    second = RenderOutput_Create(index, 0x80004000, arg1, arg2 + 32, arg3);
    second->sentinel = 0xFD;
    second->table.bits.index += 8;
    return first;
}
