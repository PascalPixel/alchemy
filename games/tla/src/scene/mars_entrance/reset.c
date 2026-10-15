#include "ride.h"

void Func_02002fc4(struct ObjectRuntime *object);

s32 Func_020031dc(struct ObjectRuntime *object)
{
    Func_02002fc4(object);
    object->x = 0;
    object->y = 0;
    object->z = 0;
    object->velocity_x = 0;
    object->velocity_y = 0;
    object->velocity_z = 0;
    return 0;
}
