#include "scene.h"
#include "abi/object/dispatch/init_from_table6.h"
#include "object_dispatch.h"

void ObjectDispatch_InitFromTable6(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)gRom);
}
