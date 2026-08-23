#include "object_runtime.h"

void Object_SetMode(struct ObjectRuntime *, s32);

void Func_080924d4(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL)
        Object_SetMode(object, action);
}
