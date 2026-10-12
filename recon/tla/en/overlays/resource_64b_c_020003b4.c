/* Near miss of Func_020003b4 (28 instructions, 2 differ): the native adds the x offset after
 * it loads z; the draft adds it before. Measured: Call4 forwarding and x, z locals the same;
 * a one-pass block before the call sinks the x load below the two stores (4 differ); x and
 * the offset behind a "+r" barrier before the stores take other registers (12 differ). */
#include "types.h"
#include "object_runtime.h"

s32 GameFlag_Test(s32 flag);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Func_02004b50(struct ObjectRuntime *object, s32 x, s32 y, s32 z);

s32 Func_020003b4(struct ObjectRuntime *object)
{
    if (!GameFlag_Test(0x200))
        return 1;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    object->acceleration = 0x1000;
    object->speed_limit = 0x1000;
    Func_02004b50(object, object->x - 0x1e0000, object->y, object->z + 0x80000);
    return 0;
}
