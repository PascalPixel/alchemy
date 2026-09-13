#include "object_runtime.h"

void Object_SetMode(struct ObjectRuntime *, s32);
void Object_CommitPosition(struct ObjectRuntime *);
void ObjectMotion_OffsetPositionAndReset(u32, s32, s32);

void ObjectMotion_CommitPositionAndActivate(u32 object_id, s32 x_offset, s32 z_offset)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    ObjectMotion_OffsetPositionAndReset(object_id, x_offset, z_offset);
    if (object != NULL) {
        Object_CommitPosition(object);
        Object_SetMode(object, 1);
    }
}
