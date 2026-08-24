#include "script_object_runtime.h"
#include "fixed_math.h"

s16 ArcTan2(s32, s32);

s32 Func_0800da78(struct ScriptObjectRuntime *object)
{
    struct ScriptObjectRuntime *target;

    target = object->linked_object;
    object->script_value = ArcTan2(
        (s32)((u32)target->z - (u32)object->z),
        (s32)((u32)target->x - (u32)object->x));
    object->script_cursor++;
    return 1;
}
