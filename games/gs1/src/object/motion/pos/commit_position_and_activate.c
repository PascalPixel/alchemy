#include "scene.h"
#include "object_runtime.h"

void Object_SetMode(struct ObjectRuntime *, s32);
void Object_CommitPosition(struct ObjectRuntime *);
void Motion_OffsetPositionAndReset(u32, s32, s32);

void Motion_CommitPositionAndActivate(u32 object_id, s32 x_offset, s32 z_offset)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    Motion_OffsetPositionAndReset(object_id, x_offset, z_offset);
    if (object != NULL) {
        Object_CommitPosition(object);
        Object_SetMode(object, 1);
    }
}
