#include "scene.h"
#include "abi/object/link/link_object_and_set_callback.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(u32);
void Motion_SetActionCallback(struct ObjectRuntime *, s32);
extern const u8 gRom[];

void Object_LinkObjectAndSetCallback(u32 object_id, u32 linked_object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->linked_object = Object_GetById(linked_object_id);
        Motion_SetActionCallback(object, (s32)gRom);
    }
}
