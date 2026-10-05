#include "OBJECT_RUNTIME.H"

void ObjectMotion_ResetTargetsAndVelocity(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        s32 value = 1;
        value |= object->action_flags;
        object->action_flags = value;
        Object_ResetMotion(object);
        object->target_x = 0x80000000;
        object->target_y = 0x80000000;
        object->target_z = 0x80000000;
        object->velocity_x = 0;
        object->velocity_y = 0;
        object->velocity_z = 0;
        object->motion_flags = 0;
    }
}
