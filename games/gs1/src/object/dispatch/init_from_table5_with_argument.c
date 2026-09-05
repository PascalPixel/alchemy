#include "object_dispatch.h"

void ObjectDispatch_InitFromTable5WithArgument(struct DispatchObject *object, s32 argument)
{
    ObjectDispatch_Initialize(object, (u32)Data_08013608);
    object->argument = argument;
}
