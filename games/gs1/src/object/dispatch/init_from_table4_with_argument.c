#include "object_dispatch.h"

void ObjectDispatch_InitFromTable4WithArgument(struct DispatchObject *object, s32 argument)
{
    ObjectDispatch_Initialize(object, (u32)Data_080135f0);
    if (argument != 0) {
        object->value_34 = 0x8000;
        object->value_30 = 0x40000;
        object->argument = argument;
        object->value_64 = 0;
    }
}
