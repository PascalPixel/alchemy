#include "OBJECT_RUNTIME.H"

struct ObjectRuntime *Object_GetById(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object == NULL)
        return NULL;
    return object;
}

void ObjectMotion_SetSpeedParameters(u32 object_id, s32 speed_limit, s32 acceleration)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->acceleration = acceleration;
        object->speed_limit = speed_limit;
    }
}

void ObjectMotion_EnableActionAndSetCallback(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        s32 value = 1;
        value |= object->action_flags;
        object->action_flags = value;
        Object_SetActionCallback(object, action);
    }
}

void ObjectMotion_EnableActionAndResetMotion(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);
    u8 enabled;

    if (object != NULL) {
        enabled = 1;
        object->action_flags = enabled | object->action_flags;
        Object_ResetMotion(object);
    }
}
