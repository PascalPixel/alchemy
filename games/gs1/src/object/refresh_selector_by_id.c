#include "object_runtime.h"

#define Object_RefreshSelectorById Func_080920e8

void Func_08009148(struct ObjectRuntime *);

void Object_RefreshSelectorById(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL)
        Func_08009148(object);
}
