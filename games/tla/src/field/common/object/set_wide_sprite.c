#include "types.h"
#include "object_runtime.h"

struct ObjectSprite {
    u8 unknown_00[5];
    u8 attr0_high;
    u8 unknown_06;
    u8 attr1_high;
    u8 unknown_08[0xf];
    u8 y_offset;
};

void Object_SetWideSprite(u32 object_id, s32 mode)
{
    struct ObjectRuntime *object;
    struct ObjectSprite *sprite;

    object = ObjectTable_Get(object_id);
    if (object == 0)
        return;
    if (mode == 0) {
        object->x = 0;
        object->z = 0;
        object->unknown_4c = 0x31415927;
        return;
    }
    sprite = object->animation;
    if (sprite == 0)
        return;
    sprite->y_offset += 4;
    sprite->attr0_high = (sprite->attr0_high & 0x3f) | 0x40;
    sprite->attr1_high &= 0x3f;
}
