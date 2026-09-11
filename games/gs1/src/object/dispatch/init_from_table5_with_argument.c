#include "scene.h"
#include "abi/object/dispatch/init_from_table5_with_argument.h"
#include "object_dispatch.h"

void ObjectDispatch_InitFromTable5WithArgument(struct DispatchObject *object, s32 argument)
{
    ObjectDispatch_Initialize(object, (u32)gRom);
    object->argument = argument;
}
