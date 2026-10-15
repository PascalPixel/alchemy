#include "shaman_village.h"

u8 *Runtime_AllocateHeapBlockFar(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);
void Func_02004884(s32 resource);

struct ShamanVillageObject *Func_02001fec(void)
{
    struct ShamanVillageObject *object = Engine_ObjectCreate(489, 0x780000, 0x400000, 0x1080000);
    u8 *buffer = NULL;

    if (object == NULL)
        return NULL;
    {
        struct FieldSprite *sprite = object->sprite;

        sprite->full_color = 0;
        sprite->palette = 0;
        sprite->priority = 1;
        object->motion_flags = 0;
        object->field_5c = 1;
        sprite->next_frame = 0;
        sprite->unknown_1b[0] = 0;
        buffer = Runtime_AllocateHeapBlockFar(68, 0x608);
        Func_02004884(65);
        VramBlock_LoadCached(sprite->vram_block, 128, buffer + 0x400);
        Runtime_ReleaseHeapBlock(68);
    }
    return object;
}
