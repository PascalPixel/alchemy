#include "object_runtime.h"

void Func_08009080(struct ObjectRuntime *, s32);
void Func_08009158(struct ObjectRuntime *);

void Func_080923c4(u32 object_id)
{
    struct ObjectRuntime *object = Func_0808ba1c(object_id);

    if (object != NULL) {
        Func_08009158(object);
        Func_08009080(object, 1);
    }
}
