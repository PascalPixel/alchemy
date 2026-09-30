#include "OBJECT_RUNTIME.H"

void Object_CommitPosition(struct ObjectRuntime *);
void Object_SetMode(struct ObjectRuntime *, s32);

void ObjectMotion_CommitCurrentPositionAndActivate(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        Object_CommitPosition(object);
        Object_SetMode(object, 1);
    }
}
