#include "object_runtime.h"

void Func_08093a6c(struct ObjectRuntime *object, s32 kind);

void Func_0809207c(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = Func_0808ba1c(object_id);

    if (object != NULL) {
        s32 value = 1;
        value |= object->action_flags;
        object->action_flags = value;
        Func_08093a6c(object, action);
    }
}
