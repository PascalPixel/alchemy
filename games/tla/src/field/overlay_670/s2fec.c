#include "ov670.h"

void Func_02002fec(struct Ov670Object *object)
{
    s32 step;

    object->x += object->field_64 << 8;
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ stores x before it loads y (6 differ without the two blocks). */
    object->y += 0x8000;
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ stores y before it builds the scale step. */
    step = 0x51e; /* FAKEMATCH: the temporary builds the scale step before scale_x is loaded (one reordered row). */
    object->scale_x += step;
    object->scale_y += step;
    object->field_64 += 2;
    if (--object->field_68 == 0)
        Func_02005a3c(object);
}
