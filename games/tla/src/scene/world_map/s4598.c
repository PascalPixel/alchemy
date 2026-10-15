#include "types.h"
#include "object_runtime.h"

s32  Math_Sine(s32 angle);
void Func_02004b38(struct ObjectRuntime *object);

void Func_02004598(struct ObjectRuntime *object)
{
    struct ObjectRuntime *parent = object->linkedObject;
    s32                   step = ++object->action;
    s32                   scale;
    s32                   rise;

    if (step > 31)
    {
        Func_02004b38(object);
        return;
    }
    scale = Math_Sine(step << 10);
    object->scaleX = scale;
    object->scaleY = scale;
    /* FAKEMATCH: the loop note ends the scheduling block and the rise is a local: the native starts the rise before it copies x and shifts it after (4 differ without them). */
    do
    {
    } while (0);
    rise = 0x10000;
    object->x = parent->x;
    object->y += rise;
    object->z = parent->z + (rise - scale) * 5 + 0x80000;
}

void Func_020045e4(struct ObjectRuntime *object)
{
    struct ObjectRuntime *parent = object->linkedObject;
    s32                   step = ++object->action;
    s32                   scale;
    s32                   rise;

    if (step > 31)
    {
        Func_02004b38(object);
        return;
    }
    scale = Math_Sine(step << 10);
    object->scaleX = scale;
    object->scaleY = -scale;
    /* FAKEMATCH: the loop note ends the scheduling block and the rise is a local: the native starts the rise before it copies x and shifts it after (4 differ without them). */
    do
    {
    } while (0);
    rise = 0x10000;
    object->x = parent->x;
    object->y += rise;
    object->z = parent->z - (rise - scale) * 5 + 0x100000;
}
