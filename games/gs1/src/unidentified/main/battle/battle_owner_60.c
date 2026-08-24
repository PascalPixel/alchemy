#include "types.h"

struct Object_08096574 {
    u8 padding000[8];
    s32 x;
    s32 y;
    s32 z;
    u8 padding014[84];
    struct Object_08096574 *target;
};

void Func_08096574(struct Object_08096574 *object)
{
    struct Object_08096574 *target = object->target;

    object->x += (target->x - object->x) / 2;
    object->y += (target->y - object->y) / 2;
    object->z += (target->z - object->z) / 2;
}
