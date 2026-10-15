#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);

void Func_02001c44(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    object->scaleX = 0xb333;
    object->scaleY = 0xb333;
    object->radius = 8;
}
