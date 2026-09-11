#include "object_runtime.h"

void Motion_SetActionCallback(struct ObjectRuntime *object, s32 kind);

void Motion_EnableActCb(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        s32 value = 1;
        value |= object->action_flags;
        object->action_flags = value;
        Motion_SetActionCallback(object, action);
    }
}
