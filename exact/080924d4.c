#include "object_runtime.h"

void Func_08009080(struct ObjectRuntime *, s32);

void Func_080924d4(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = GetObject(object_id);

    if (object != NULL)
        Func_08009080(object, action);
}
