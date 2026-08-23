#include "object_runtime.h"

void Object_SetAction(struct ObjectRuntime *, s32);

void Func_080924ec(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = GetObject(object_id);

    if (object != NULL)
        Object_SetAction(object, action);
}
