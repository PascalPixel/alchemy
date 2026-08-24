#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(u32);
void ObjectMotion_SetActionCallback(struct ObjectRuntime *, s32);
extern const u8 Data_0809ff40[];

void Object_LinkObjectAndSetCallback(u32 object_id, u32 linked_object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->linked_object = Object_GetById(linked_object_id);
        ObjectMotion_SetActionCallback(object, (s32)Data_0809ff40);
    }
}
