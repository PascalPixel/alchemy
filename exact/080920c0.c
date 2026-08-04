#include "object_runtime.h"

struct ObjectRuntime *Func_08092054(u32);
void Func_08093a6c(struct ObjectRuntime *, s32);

void Func_080920c0(u32 object_id, u32 linked_object_id)
{
    struct ObjectRuntime *object = Func_0808ba1c(object_id);

    if (object != NULL) {
        object->linked_object = Func_08092054(linked_object_id);
        Func_08093a6c(object, 0x0809FF40);
    }
}
