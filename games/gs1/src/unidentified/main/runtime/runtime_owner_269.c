#include "object_dispatch.h"

void Func_0800c454(struct DispatchObject *object, s32 argument)
{
    ObjectDispatch_Initialize(object, (u32)Data_08013608);
    object->argument = argument;
}
