/* Near miss, differ 2: the listing loads object->z before it builds 0x26c0000
 * (ldr r0, [r0, #16]; movs r3, #155); this draft builds the constant first.
 * Separate ifs and a z temporary score worse (12 and 21). */
#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);

s32 Func_02000038(void)
{
    struct ObjectRuntime *object = Object_GetById(10);

    if ((u32)(object->x - 0x940000) <= 0x80000 && object->z >= 0x26c0000 && object->z <= 0x2740000)
        return 0;
    return 1;
}
