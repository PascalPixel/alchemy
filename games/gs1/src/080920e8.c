#include "object_runtime.h"

void Func_08009148(struct ObjectRuntime *);

void Func_080920e8(u32 object_id)
{
    struct ObjectRuntime *object = GetObject(object_id);

    if (object != NULL)
        Func_08009148(object);
}
