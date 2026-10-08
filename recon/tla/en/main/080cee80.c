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

void Object_SetSquareSprite(u32 object_id)
{
    struct ObjectRuntime *object;
    struct ObjectSprite *sprite;

    object = ObjectTable_Get(object_id);
    if (object == 0)
        return;
    sprite = object->animation;
    if (sprite == 0)
        return;
    sprite->y_offset = 8;
    sprite->attr0_high &= 0x3f;
    sprite->attr1_high = (sprite->attr1_high & 0x3f) | 0x40;
}
