#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);

void Func_02001c44(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    object->scale_x = 0xb333;
    object->scale_y = 0xb333;
    object->radius = 8;
}
