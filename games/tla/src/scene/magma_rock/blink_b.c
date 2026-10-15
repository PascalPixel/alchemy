#include "types.h"
#include "frame.h"
#include "object_runtime.h"

void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);

void Func_020020d0(struct ObjectRuntime *object)
{
    if (gFrameCount & object->unknown_5c[7])
        Object_SetPartAttribute(object, 7);
    else
        Object_SetPartAttribute(object, 0);
}
