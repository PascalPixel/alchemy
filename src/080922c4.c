#include "object_runtime.h"

void Func_08009080(struct ObjectRuntime *, s32);
void Func_08009140(struct ObjectRuntime *);
void Func_08009150(struct ObjectRuntime *, s32, s32, s32);

void Func_080922c4(u32 object_id, s32 x_offset, s32 z_offset)
{
    struct ObjectRuntime *object = Func_0808ba1c(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        Func_08009140(object);
        Func_08009080(object, 2);
        Func_08009150(object, object->x + (x_offset << 16),
            object->y, object->z + (z_offset << 16));
    }
}
