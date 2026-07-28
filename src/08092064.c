#include "object_runtime.h"

void Func_08092064(u32 object_id, s32 speed_limit, s32 acceleration)
{
    struct ObjectRuntime *object = Func_0808ba1c(object_id);

    if (object != NULL) {
        object->acceleration = acceleration;
        object->speed_limit = speed_limit;
    }
}
