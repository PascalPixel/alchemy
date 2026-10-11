/* Near miss, differ 2: as Func_02000038, the listing loads object->z before it
 * builds 0x26c0000; this draft builds the constant first. */
#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);

s32 Func_0200006c(void)
{
    struct ObjectRuntime *object = Object_GetById(9);

    if ((u32)(object->x - 0x840000) <= 0x80000 && object->z >= 0x26c0000 && object->z <= 0x2740000)
        return 0;
    return 1;
}
