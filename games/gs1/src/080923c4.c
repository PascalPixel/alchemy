#include "object_runtime.h"

void Object_SetMode(struct ObjectRuntime *, s32);
void Object_CommitPosition(struct ObjectRuntime *);

void Func_080923c4(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        Object_CommitPosition(object);
        Object_SetMode(object, 1);
    }
}
