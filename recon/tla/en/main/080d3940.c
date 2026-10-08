/* Near miss: alchemy diff 080d3940,080d3a10,080d3b28 differs by store and
 * constant scheduling around the position copy and the zero/0x400 register
 * choice in Func_080d3b28. */
#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"
#include "dma.h"

struct ObjectRuntime *Object_GetById(u32);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object, u32 value);
void SetMapCellCollisionFar(s32 terrain, s32 x, s32 z, s32 value);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
u8 *Runtime_AllocateHeapBlock(s32 block, s32 size);
void Runtime_ReleaseHeapBlock(s32 block);
void Func_08038248(s32 resource, u8 *buffer);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);
void ResourceMetadata_ClearRecordFar(void *record);

struct ObjectSprite {
    u8 unknown_00[5];
    u8 attr0_high;
    u8 unknown_06[2];
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u8 unknown_0a[6];
    u8 vram_block;
    u8 unknown_11;
    u16 rotation;
    u8 unknown_14[2];
    s8 offset_x;
    s8 offset_y;
    u8 frame;
    u8 frame_timer;
    u8 next_frame;
    u8 unknown_1b;
    u8 unknown_1c[0xc];
    void *record;
};

void Func_080d3940(u32 id, u32 angle)
{
    s32 position[3];
    struct ObjectRuntime *object = Object_GetById(id);
    struct ObjectSprite *sprite = object->animation;

    if (object != NULL && sprite != NULL) {
        Object_SetMode(object, 0);
        angle = (angle + 0x2000) & 0xc000;
        position[0] = object->x;
        position[2] = object->z;
        SetMapCellCollisionFar(object->terrain_id, position[0], position[2], 255);
        Vector_AddPolarOffset(0x100000, angle, position);
        SetMapCellCollisionFar(object->terrain_id, position[0], position[2], 255);
        switch (angle) {
        case 0:
            sprite->offset_x -= 3;
            sprite->offset_y += 5;
            break;
        case 0x8000:
            sprite->offset_x += 2;
            sprite->offset_y += 4;
            break;
        case 0xc000:
            sprite->offset_y += 1;
            break;
        case 0x4000:
            sprite->offset_y += 5;
            break;
        }
        object->collision_flags = 0;
        object->priority_flags |= 2;
        sprite->next_frame = 0;
        sprite->rotation = angle + 0x4000;
    }
}

void Func_080d3a10(u32 id, u32 angle, s32 mode)
{
    s32 position[3];
    struct ObjectRuntime *object = Object_GetById(id);
    struct ObjectSprite *sprite = object->animation;

    if (object != NULL && sprite != NULL) {
        if (mode != -1)
            Object_SetMode(object, mode);
        position[0] = object->x;
        angle = (angle + 0x2000) & 0xc000;
        position[2] = object->z;
        SetMapCellCollisionFar(object->terrain_id, position[0], position[2], 255);
        Vector_AddPolarOffset(0x100000, angle, position);
        SetMapCellCollisionFar(object->terrain_id, position[0], position[2], 255);
        switch (angle) {
        case 0:
            object->x += 0x80000;
            object->z += 0x10000;
            sprite->offset_x -= 4;
            sprite->offset_y += 15;
            break;
        case 0x8000:
            object->x += -0x80000;
            object->z += 0x10000;
            sprite->offset_x += 3;
            sprite->offset_y += 16;
            break;
        case 0xc000:
            sprite->offset_y += 1;
            break;
        case 0x4000:
            object->z += 0x80000;
            sprite->offset_y += 13;
            break;
        }
        object->collision_flags = 0;
        object->priority_flags |= 2;
        sprite->next_frame = 0;
        sprite->rotation = angle + 0x4000;
    }
}

void Func_080d3b28(u32 id, s32 resource)
{
    struct ObjectRuntime *object = Object_GetById(id);
    struct ObjectSprite *sprite;
    u8 *buffer;
    s32 tile;
    u8 kind;

    if (object != NULL) {
        kind = object->animation_kind;
        if (kind == 1) {
            sprite = object->animation;
            if (resource & 0x8000)
                resource = 0;
            buffer = Runtime_AllocateHeapBlock(68, 0x608) + 0x400;
            Dma_Fill32(0, buffer, 0x80);
            Func_08038248(resource, buffer);
            tile = VramBlock_LoadCached(sprite->vram_block, 0x80, buffer);
            Runtime_ReleaseHeapBlock(68);
            object->unknown_5c[0] = kind;
            ResourceMetadata_ClearRecordFar(sprite->record);
            sprite->record = NULL;
            sprite->unknown_1b = 0;
            sprite->attr0_high &= ~0x20;
            sprite->tile = tile;
            sprite->frame_timer = 0;
            sprite->next_frame = 0;
        }
    }
}
