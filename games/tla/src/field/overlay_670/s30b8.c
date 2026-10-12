#include "ov670.h"

void Func_020030b8(struct Ov670Object *object)
{
    if (object->field_64 != 0) {
        object->field_64--;
    } else {
        object->field_64 = ((Random16Far() * 10) >> 16) + 10;
        do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ stores the new count before it takes the next field's address (2 differ without it). */
        if (object->field_66 != 0) {
            object->scale_x = 0x18000;
            object->field_66 = 0;
        } else {
            object->scale_x = 0xfffe8000;
            object->field_66 = 1;
        }
    }
}

void Func_02003104(s32 x, s32 y, s32 z)
{
    struct Ov670Object *object = Func_02005a34(0x106, x, y, z);
    struct FieldSprite *sprite;
    void (*update)(struct Ov670Object *);
    u32 count;

    if (object != 0) {
        object->motion_flags = 0;
        do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: the native clears the motion flags before it takes the priority flags' address (2 differ without it). */
        object->priority_flags &= ~1;
        object->scale_y = 0x20000;
        count = (Random16Far() * 10) >> 16;
        /* FAKEMATCH: the native finishes the random count before it takes the field's address; the plain store takes the address first (one reordered pair). */
        asm volatile("");
        object->field_64 = count + 10;
        object->field_66 = 0;
        update = Func_020030b8;
        /* FAKEMATCH: the native loads the callback before the sprite; the plain order loads the sprite first (one reordered pair). */
        asm volatile("" : "+l"(update));
        sprite = object->sprite;
        object->update = update;
        sprite->priority = 1;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        Func_02005a24(object, 5);
        Animation_SetStateFlags(object, 1);
    }
}
