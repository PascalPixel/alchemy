#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"

struct ObjectRuntime *Func_02001f0c(s32 type, s32 x, s32 y, s32 z);
void Func_02001f4c(s32 a);
u8 *Runtime_AllocateHeapBlockFar(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
s32 VramBlock_LoadCached(s32 block, s32 count, u8 *source);

struct ObjectRuntime *Func_02001210(void)
{
    struct ObjectRuntime *object = Func_02001f0c(0x1e9, 0x1680000, 0x400000, 0x1a80000);
    u8 *buffer = 0;
    struct FieldSprite *sprite;

    if (object == 0)
        return 0;
    sprite = object->animation;
    sprite->full_color = 0;
    sprite->palette = 0;
    sprite->priority = 1;
    object->motion_flags = 0;
    object->unknown_5c[0] = 1;
    sprite->next_frame = 0;
    sprite->unknown_1b[0] = 0;
    buffer = Runtime_AllocateHeapBlockFar(68, 0x608);
    Func_02001f4c(242);
    VramBlock_LoadCached(sprite->vram_block, 128, buffer + 0x400);
    Runtime_ReleaseHeapBlock(68);
    return object;
}
