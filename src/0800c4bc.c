#include "object_dispatch.h"

void Func_0800c4bc(struct DispatchObject *object, s32 argument)
{
    Func_0800c2d8(object, 0x080135F0);
    if (argument != 0) {
        object->value_34 = 0x8000;
        object->value_30 = 0x40000;
        object->argument = argument;
        object->value_64 = 0;
    }
}
