#include "object_runtime.h"

#define Object_SetActionCallbackAndRefreshById Func_080920fc

void ObjectMotion_SetActionCallback(struct ObjectRuntime *, s32);
void Func_08009148(struct ObjectRuntime *);

void Object_SetActionCallbackAndRefreshById(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        s32 value = 1;
        value |= object->action_flags;
        object->action_flags = value;
        ObjectMotion_SetActionCallback(object, action);
        Func_08009148(object);
    }
}
