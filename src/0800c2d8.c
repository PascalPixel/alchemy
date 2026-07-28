#include "object_dispatch.h"

void Func_0800c2d8(struct DispatchObject *object, u32 value)
{
    if (object != 0) {
        object->value_04 = 0;
        object->value_00 = value;
        object->value_5b = 0;
        object->value_5d = 0;
        object->value_57 = 0;
    }
}
