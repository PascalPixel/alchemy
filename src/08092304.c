#include "object_runtime.h"

void Func_08009080(struct ObjectRuntime *, s32);
void Func_08009158(struct ObjectRuntime *);
void Func_080922c4(u32, s32, s32);

void Func_08092304(u32 object_id, s32 x_offset, s32 z_offset)
{
    struct ObjectRuntime *object = Func_0808ba1c(object_id);

    Func_080922c4(object_id, x_offset, z_offset);
    if (object != NULL) {
        Func_08009158(object);
        Func_08009080(object, 1);
    }
}
