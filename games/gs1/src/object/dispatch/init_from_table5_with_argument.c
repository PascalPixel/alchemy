#include "object_dispatch.h"

#define ObjectDispatch_InitFromTable5WithArgument Func_0800c454

void ObjectDispatch_InitFromTable5WithArgument(struct DispatchObject *object, s32 argument)
{
    ObjectDispatch_Initialize(object, (u32)Data_08013608);
    object->argument = argument;
}
