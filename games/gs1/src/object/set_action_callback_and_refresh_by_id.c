#include "scene.h"
#include "abi/object/set_action_callback_and_refresh_by_id.h"
#include "object_runtime.h"

void Motion_SetActionCallback(struct ObjectRuntime *, s32);

void Object_SetActionCallbackAndRefreshById(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        s32 value = 1;
        value |= object->action_flags;
        object->action_flags = value;
        Motion_SetActionCallback(object, action);
        Obj_Do(object);
    }
}
