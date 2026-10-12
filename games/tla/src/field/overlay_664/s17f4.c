#include "ov664.h"

void Func_020017f4(struct FieldActor *object)
{
    struct FieldActor *target = (struct FieldActor *)WORD_AT(object, 104);
    s32 lift;

    object->priority_flags ^= 2 << (gFrameCount & 1);
    lift = 128;
    asm volatile("" : "+l"(lift)); /* FAKEMATCH: the listing starts the lift constant before it loads the target x and shifts it before the store; as a plain constant it starts after the load and is shifted after the y load (differ 4) */
    object->x = target->x;
    object->y = target->y + (lift << 12);
    object->z = target->z;
}
