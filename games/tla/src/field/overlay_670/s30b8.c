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
