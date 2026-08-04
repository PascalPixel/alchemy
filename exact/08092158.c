#include "object_runtime.h"

void Func_08009140(struct ObjectRuntime *);
void Func_08009150(struct ObjectRuntime *, s32, s32, s32);
void Func_08009158(struct ObjectRuntime *);

void Func_08092158(u32 object_id, s32 x, s32 z)
{
    struct ObjectRuntime *object = Func_0808ba1c(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        Func_08009140(object);
        Func_08009150(object, x << 16, object->y, z << 16);
        Func_08009158(object);
    }
}
