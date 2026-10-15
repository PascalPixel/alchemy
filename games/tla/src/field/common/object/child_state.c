#include "object_dispatch.h"

void ObjectDispatch_InitFromTable6(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)gObjectDispatch_Table6);
}

void ObjectDispatch_InitFromTable4WithArgument(struct DispatchObject *object, s32 argument)
{
    ObjectDispatch_Initialize(object, (u32)gObjectDispatch_Table4);
    if (argument != 0) {
        object->value34 = 0x8000;
        object->value30 = 0x40000;
        object->argument = argument;
        object->value64 = 0;
    }
}
