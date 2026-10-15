#include "airs_rock_inside.h"

void Func_020017f4(struct FieldActor *object)
{
    struct FieldActor *target = (struct FieldActor *)WORD_AT(object, 104);
    s32                lift;

    object->priorityFlags ^= 2 << (gFrameCount & 1);
    lift = 128;
    /* FAKEMATCH: the listing starts the lift constant before it loads the target x and shifts it before the store; as a plain constant it starts after the load and is shifted after the y load (differ 4) */
    asm volatile("" : "+l"(lift));
    object->x = target->x;
    object->y = target->y + (lift << 12);
    object->z = target->z;
}
