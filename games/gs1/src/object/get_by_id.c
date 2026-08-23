#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(u32 object_id)
{
    struct ObjectRuntime *object = GetObject(object_id);

    if (object == NULL)
        return NULL;
    return object;
}
