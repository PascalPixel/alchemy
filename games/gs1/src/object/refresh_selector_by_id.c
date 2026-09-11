#include "scene.h"
#include "abi/object/refresh_selector_by_id.h"
#include "object_runtime.h"

void Object_RefreshSelectorById(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL)
        Obj_Do(object);
}
