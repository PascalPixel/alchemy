#include "object_runtime.h"

void Func_08093a6c(struct ObjectRuntime *, s32);
void Func_08009148(struct ObjectRuntime *);

void Func_080920fc(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = Func_0808ba1c(object_id);

    if (object != NULL) {
        s32 value = 1;
        value |= object->action_flags;
        object->action_flags = value;
        Func_08093a6c(object, action);
        Func_08009148(object);
    }
}
