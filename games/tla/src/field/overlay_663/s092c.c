#include "types.h"
#include "object_runtime.h"

extern u32 gFrameCount;

void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);

void Func_0200092c(struct ObjectRuntime *object)
{
    u8 *bytes = (u8 *)object;
    u32 on = ((gFrameCount & 3) >> bytes[99]) & 1;

    Object_SetPartAttribute(object, (u8)(on * bytes[98]));
}
