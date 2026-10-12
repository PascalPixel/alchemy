#include "types.h"
#include "object_dispatch.h"

extern const s32 Data_0802f1e8[];

void Func_08023510(struct DispatchObject *object, s32 argument)
{
    ObjectDispatch_Initialize(object, Data_0802f1e8);
    object->argument = argument;
}
