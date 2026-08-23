#include "object_runtime.h"

void Object_SetMode(struct ObjectRuntime *, s32);
void Func_08009158(struct ObjectRuntime *);

void Func_080923c4(u32 object_id)
{
    struct ObjectRuntime *object = GetObject(object_id);

    if (object != NULL) {
        Func_08009158(object);
        Object_SetMode(object, 1);
    }
}
