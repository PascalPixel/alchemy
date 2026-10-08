#include "object_runtime.h"

void ObjectMotion_ResetTargetsAndVelocity(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->action_flags = (u8)(object->action_flags | 1);
        Object_ResetMotion(object);
        object->target_x = 0x80000000;
        object->target_y = 0x80000000;
        object->target_z = 0x80000000;
        object->velocity_x = 0;
        object->velocity_y = 0;
        object->velocity_z = 0;
        object->unknown_56 = 0;
    }
}
