#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(u32);
void Func_08093a6c(struct ObjectRuntime *, s32);
extern const u8 Data_0809ff40[];

void Func_080920c0(u32 object_id, u32 linked_object_id)
{
    struct ObjectRuntime *object = GetObject(object_id);

    if (object != NULL) {
        object->linked_object = Object_GetById(linked_object_id);
        Func_08093a6c(object, (s32)Data_0809ff40);
    }
}
