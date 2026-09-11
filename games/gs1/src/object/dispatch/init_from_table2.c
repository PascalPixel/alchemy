#include "scene.h"
#include "abi/object/dispatch/init_from_table2.h"
#include "object_dispatch.h"

void ObjectDispatch_InitFromTable2(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)gRom);
}
