#include "TYPES.H"
#include "OBJECT_RUNTIME.H"

s32 Func_080eab70(struct ObjectRuntime *object)
{
    return object->x / 0x100000 + (object->z / 0x100000) * 128;
}
