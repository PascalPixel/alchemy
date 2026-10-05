/* Near miss: differ 2 (20/20 instructions), one ARM9 scheduling swap. The
   listing loads attr0_high (ldrb r1, [r0, #5]) before building 8 for
   y_offset (movs r3, #8); this draft builds 8 first. Writing the stores in
   other orders, as plain assignments or with attr0 read into a local first
   (18) does not move it. Goes before SPRITE_SHAPE.C's Object_GetSpriteShape. */
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"

struct ObjectSprite {
    u8 unknown_00[5];
    u8 attr0_high;
    u8 unknown_06;
    u8 attr1_high;
    u8 unknown_08[0xf];
    u8 y_offset;
};

/* Turns the object's sprite into a square of the next size, eight pixels
   lower. */
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
