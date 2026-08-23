#include "object_runtime.h"

void Object_SetMode(struct ObjectRuntime *, s32);
void Func_08009140(struct ObjectRuntime *);
void Func_08009150(struct ObjectRuntime *, s32, s32, s32);
void Func_08009158(struct ObjectRuntime *);

void Func_080921c4(u32 object_id, s32 x, s32 z)
{
    struct ObjectRuntime *object = GetObject(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        Func_08009140(object);
        Object_SetMode(object, 2);
        Func_08009150(object, x << 16, object->y, z << 16);
        Func_08009158(object);
        Object_SetMode(object, 1);
    }
}
