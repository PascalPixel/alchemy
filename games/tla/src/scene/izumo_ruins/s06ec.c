#include "izumo_ruins.h"

void Func_020006ec(struct ObjectRuntime *object, struct ObjectRuntime *target)
{
    Object_SetPositionAndResetMotion(object, target->x, target->y, target->z);
    Object_SetPositionAndResetMotion(target, 0, 0, 0);
}
