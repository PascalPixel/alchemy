#include "ov696.h"

void Func_020042c8(struct Ov696Object *object)
{
    s32 step;

    object->x += object->field_64 << 8;
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: the native loads y before it builds 0x8000 (2 differ without it). */
    object->y += 0x8000;
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: the native stores y before it starts the step, and loads scale_x between the step's first byte and its shift (4 differ without it; the step as a local alone changes nothing). */
    step = 0x51e;
    object->scale_x += step;
    object->scale_y += step;
    object->field_64 += 2;
    if (--object->field_68 == 0)
        Func_0200481c(object);
}
