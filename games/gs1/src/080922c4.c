#include "object_runtime.h"

void Object_SetMode(struct ObjectRuntime *, s32);
void Object_ResetMotion(struct ObjectRuntime *);
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);

void Func_080922c4(u32 object_id, s32 x_offset, s32 z_offset)
{
    struct ObjectRuntime *object = GetObject(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        Object_ResetMotion(object);
        Object_SetMode(object, 2);
        Object_SetPosition(object, object->x + (x_offset << 16),
            object->y, object->z + (z_offset << 16));
    }
}
